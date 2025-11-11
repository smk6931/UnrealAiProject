from db_config import get_connection, get_cursor
import sys
import os

import psycopg2.extras
sys.path.append(os.path.abspath(
    os.path.join(os.path.dirname(__file__), '../../')))


def add_monster_columns():
    conn, cur = get_cursor()

    create_query = """
    CREATE TABLE IF NOT EXISTS monsters (
        id SERIAL PRIMARY KEY,
        name VARCHAR(100),
        level INT,
        hp INT,
        attack INT,
        habitat VARCHAR(100),
        description TEXT,
        embedding vector(1536)
    );
    """

    try:
        cur.execute(create_query)
        conn.commit()
        print("몬스터 테이블 생성 완료")
    except Exception as e:
        print("몬스터 테이블 생성 실패", e)
    finally:
        cur.close()
        conn.close()


if __name__ == "__main__":
    add_monster_columns()


def add_drop_item_column():
    conn, cur = get_cursor()
    cur.execute("""
        ALTER TABLE monsters
        ADD COLUMN IF NOT EXISTS drop_item_ids INT[];
    """)
    conn.commit()
    cur.close()
    conn.close()
    print("✅ monsters 테이블에 drop_item_ids 칼럼 추가 완료")


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
