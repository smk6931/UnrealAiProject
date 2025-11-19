import json
from unittest import result

from openai import embeddings
from db_config import get_connection, get_cursor, put_connection
import sys
import os

import psycopg2.extras

from service.graph.graph_generate import generate_graph_nodes
from service.graph.graph_linker import link_graph_nodes
from service.graph.graph_service import insert_graph_edge, insert_graph_node
from service.world.world_embed import embed_text
from service.world.world_generate import generate_new_world, polish_world
sys.path.append(os.path.abspath(
    os.path.join(os.path.dirname(__file__), '../../')))

def get_random_worlds():
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cur.execute("""
        SELECT id, title, content, metadata
        FROM worlds
        ORDER BY RANDOM()
        LIMIT 1;
    """)
    world = cur.fetchone()
    cur.close()
    put_connection(conn)

    print(f"선택된 월드{world['title']}")
    return [dict(world)]


def select_worlds_all():
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cur.execute("""
        select id, title, content, metadata
        from worlds;
    """)
    rows = cur.fetchall()
    cur.close()
    put_connection(conn)

    result = []
    for row in   rows:
        row = dict(row)
        if isinstance(row["metadata"], dict):
            row["metadata"] = json.dumps(row["metadata"], ensure_ascii=False)
        result.append(row)
    return result

#LangChain GoGo
def search_similar_worlds(embedding):
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cur.execute("""
        select id, title, content, metadata
        from worlds
        order by embedding <#> %s::vector
        limit 3;
    """, (embedding,))
    rows = cur.fetchall()
    cur.close()
    put_connection(conn)
    
    return rows

def insert_world(title, content, metadata, embedding):
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
        insert into worlds (title, content, metadata, embedding)
        values (%s,%s,%s,%s::vector)
        returning id
    """, (title, content, json.dumps(metadata), embedding))
    new_id = cur.fetchone()["id"]
    conn.commit()
    cur.close()
    put_connection(conn)
    return new_id

def generate_world_pipeline(prompt: str):
    # polished = polish_world(prompt)

    polished = prompt
    embedding = embed_text(polished)
    similar = search_similar_worlds(embedding)
    final_world = generate_new_world(polished, similar)

    metadata = {
        "prompt" : prompt,
        "polished" : polished,
        "related" : [sim["id"] for sim in similar]
    }

    world_id = insert_world(
        title  =  final_world["title"],
        content = final_world["content"],
        metadata = metadata,
        embedding = embedding
    )

    nodes = json.loads(generate_graph_nodes(final_world))
    edges = json.loads(link_graph_nodes(nodes))

    node_map = {}
    node_ids = []

    for node in nodes:
        node_id = insert_graph_node(node, world_id)
        node_map[node["name"]] = node_id
        node_ids.append(node_id)

    print ("node_ids", node_ids)
    
    for edge in edges:
        from_id = node_map[edge["from"]]
        to_id = node_map[edge["to"]]

        insert_graph_edge({
            "from_id": from_id,
            "to_id": to_id,
            "relation": edge["relation"]
        })

    world = {
        "id" : world_id,
        "content": final_world,
        "similar": similar,
        "graph_nodes": node_ids
    }

    print ("generate_world_pipeline 최종 리턴 값은", world)
    return world
