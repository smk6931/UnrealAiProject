from db_config import get_connection, get_cursor
import sys
import os

import psycopg2.extras
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
    conn.close()

    print(f"선택된 월드{world["title"]}")
    return [dict(world)]

def select_worlds_all():
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cur.execute("""
        select (id, title, content, metadata
        from worlds
    """)
    rows = cur.fetchall()
    cur.close()
    conn.close()

    return [dict(row for row in rows)]