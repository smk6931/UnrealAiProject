import base64
import psycopg2.extras
from db_config import get_cursor
import sys
import os

sys.path.append(os.path.abspath(
    os.path.join(os.path.dirname(__file__), '../..')))


def create_items_table():
    conn, cur = get_cursor()

    cur.execute("""
    CREATE TABLE IF NOT EXISTS items (
        id SERIAL PRIMARY KEY,
        world_id INT REFERENCES worlds(id) ON DELETE SET NULL,
        monster_id INT REFERENCES monsters(id) ON DELETE SET NULL,
        name VARCHAR(100) NOT NULL,
        rarity VARCHAR(50),
        type VARCHAR(50),
        description TEXT,
        embedding VECTOR(1536),
        created_at TIMESTAMP DEFAULT NOW()
    );
    """)
    conn.commit()
    cur.close()
    conn.close()


def select_items_all():
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cur.execute("""
        select 
        id, world_id, monster_id, name, rarity, type, description, image_url
        from items;
    """)
    conn.commit()
    rows = cur.fetchall()

    cur.close()
    conn.close()
    return [dict(row) for row in rows]

if __name__ == "__main__":
    create_items_table()
