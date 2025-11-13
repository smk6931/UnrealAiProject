

import json

import psycopg2.extras
from db_config import get_cursor
from service.graph.graph_embed import embed_node


def insert_graph_node(node, world_id):
    conn, cur = get_cursor()
    emb = embed_node(node["description"])
    node["world_id"] = world_id

    cur.execute("""
      insert into graph_nodes (node_type, name, description, embedding, metadata)
      values (%s, %s, %s, %s, %s)
      returning id;
              
    """, (node["node_type"], node["name"], node["description"],
          emb, json.dumps(node, ensure_ascii=False)))
    new_id = cur.fetchone()['id']
    conn.commit()
    
    cur.close()
    conn.close()
    return new_id

def insert_graph_edge(edge, node_ids):
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    from_id = node_ids[edge["from"]]
    to_id = node_ids[edge["to"]]

    cur.execute("""
       insert into graph_edges (from_id, to_id, relation, metadata)
       values (%s,%s,%s,%s)
    """, (
         from_id, to_id, edge["relation"], json.dumps(edge)
    ))
    conn.commit()
    cur.close()
    conn.close()
    
