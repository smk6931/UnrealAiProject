from db_config import get_cursor
import sys
import os
sys.path.append(os.path.abspath(
    os.path.join(os.path.dirname(__file__), '../../')))


def clear_worlds_table():
    conn, cur = get_cursor()
    try:
        cur.execute("DELETE FROM worlds;")  # 전체 데이터 삭제
        conn.commit()
        print(" worlds 테이블의 모든 데이터가 삭제되었습니다.")
    except Exception as e:
        print(" 데이터 삭제 실패:", e)
    finally:
        cur.close()
        conn.close()


def create_worlds_table():
    conn, cur = get_cursor()

    cur.execute("""
    create table worlds(
      id serial primary key,
      title varchar(255) not null,
      content text not null,
      metadata jsonb,
      embedding vector(1536),
      created_at timestamp default now()
    );
    """)

    # cur.execute("drop table if exists worlds;")

    conn.commit()
    print("world_main 테이블 생성 완료")
    cur.close()
    conn.close()