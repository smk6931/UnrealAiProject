

import json
from db_config import get_cursor
from service.graph.graph_embed import embed_node


def insert_node(node):
    conn, cur = get_cursor()
    emb = embed_node(node["description"])
    cur.execute("""
      insert into graph_nodes (node_type, name, description, embedding, metadata)
      values (%s, %s, %s, %s, %s)
      returning id;
              
    """, (node["node_type"], node["name"], node["description"],
          emb, json.dumps(node, ensure_ascii=False)))
    new_id = cur.fetchone()['id']
    conn.commit()
