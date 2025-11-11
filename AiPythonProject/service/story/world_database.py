from db_config import get_connection, get_cursor
import sys
import os

import psycopg2.extras
sys.path.append(os.path.abspath(
    os.path.join(os.path.dirname(__file__), '../../')))

def get_random_world_story():
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cur.execute("""
        SELECT id, title, content, metadata
        FROM world_story
        ORDER BY RANDOM()
        LIMIT 1;
    """)
    world = cur.fetchone()
    cur.close()
    conn.close()

    print(f"선택된 월드{world["title"]}")
    return [dict(world)]
