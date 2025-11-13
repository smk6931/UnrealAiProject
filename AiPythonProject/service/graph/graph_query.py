
import psycopg2.extras
from db_config import get_cursor


def graph_context(node_id):
  conn, cur = get_cursor()
  cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
  cur.execute("""
     select * from graph_edges ge
     join graph_nodes gn ON gn.id = ge.to_id
     where ge.from_id = %s;
  """, (node_id,))
  rows = cur.fetchall()

  cur.close()
  conn.close()
  return rows