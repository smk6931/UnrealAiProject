from db_config import get_connection, get_cursor, put_connection


def insert_monsters():
    conn, cur = get_cursor()
    if conn is None:
        return

    insert_query = """
    insert into monsters (name, level, hp, attack, habitat, description)
    values('슬라임', 1, 50, 5, '숲', '젤리처럼 말랑한 초보 몬스터로, 주로 숲속의 그늘진 곳에 서식한다.'),
        ('고블린', 3, 120, 15, '동굴', '작고 민첩한 괴물로, 어두운 동굴이나 폐허 근처에서 활동한다.'),
        ('트롤', 7, 300, 30, '산지', '큰 체격과 강력한 힘을 지닌 몬스터로, 산악 지대에 주로 서식한다.'),
        ('드래곤', 15, 1200, 80, '화산', '고대의 강력한 생명체로, 화산지대 깊은 곳에서 잠든다.')
    """

    try:
        cur.execute(insert_query)
        conn.commit()
        print("몬스터 기본 정보 + 설명 + 서식지 삽입 완료")
    except Exception as e:
        print("몬스터 데이터 삽입 실패" + e)
    finally:
        cur.close()
        put_connection(conn)

if __name__ == "__main__":
    insert_monsters()