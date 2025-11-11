from db_config import get_connection, get_cursor
import sys
import os

import psycopg2.extras
sys.path.append(os.path.abspath(
    os.path.join(os.path.dirname(__file__), '../../')))


def add_monster_columns():
    conn, cur = get_cursor()
    cur.execute("""
    CREATE TABLE IF NOT EXISTS monsters (
        id SERIAL PRIMARY KEY,
        name VARCHAR(100),
        level INT,
        hp INT,
        attack INT,
        habitat VARCHAR(100),
        description TEXT,
        embedding vector(1536),
        drop_item_ids INT
    );
    """)
    conn.commit()

    cur.close()
    conn.close()


def select_monsters_all():
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
      SELECT 
            name,
            level,
            hp,
            attack,
            habitat,
            description,
            image_url,
            drop_item_ids
        FROM monsters;
    """)
    rows = cur.fetchall()

    cur.close()
    conn.close()

    return [dict(row) for row in rows]

def select_items(item_ids: list[int]):
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
        select id, world_id, monster_id, name, rarity, type, description, image_url
        from items
        where id = any(%s);            
    """,(item_ids,))

    rows = cur.fetchall()
    cur.close()
    conn.close()

    return [dict(row) for row in rows]

def select_monsters(monster_ids: list[int]):
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
        select   name,
            level, hp,
            attack,
            habitat,
            description,
            image_url,
            drop_item_ids
        from monsters
        where id = any(%s);
    """, (monster_ids,))

    rows = cur.fetchall()
    cur.close()
    conn.close()

    return[dict(row) for row in rows]
