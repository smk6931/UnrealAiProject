import sys, os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../')))

from db_config import get_cursor


def clear_world_story_table():
    conn, cur = get_cursor()
    try:
        cur.execute("DELETE FROM world_story;")  # 전체 데이터 삭제
        conn.commit()
        print(" world_story 테이블의 모든 데이터가 삭제되었습니다.")
    except Exception as e:
        print(" 데이터 삭제 실패:", e)
    finally:
        cur.close()
        conn.close()

if __name__ == "__main__":
    clear_world_story_table()

def create_world_story_table():
    conn, cur = get_cursor()

    create_table_query = """
    create table world_story(
      id serial primary key,
      title varchar(255) not null,
      content text not null,
      metadata jsonb,
      embedding vector(1536),
      created_at timestamp default now()
    );
    """
    try:
        # cur.execute("drop table if exists world_story;")
        cur.execute(create_table_query)
        conn.commit()
        print("world_main 테이블 생성 완료")
    except Exception as e:
        print("테이블 생성 실패:", e)
    finally:
        cur.close()
        conn.close()

if __name__ == "__main__":
    create_world_story_table()
