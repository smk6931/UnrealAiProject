
from openai import OpenAI
import psycopg2
import psycopg2.extras
from db_config import get_cursor, put_connection

client = OpenAI()

def generate_embedding(text):
    response = client.embeddings.create(
        model="text-embedding-3-small",
        input=text
    )
    return response.data[0].embedding

def update_monster_embeddings():
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    if conn is None:
        return

    cur.execute(
        "select id, name, level, hp, attack, habitat, description from monsters;")
    monsters = cur.fetchall()

    for monster in monsters:
        print("몬스터 객체 정보", dict(monster))
        id, name, level, hp, attack, habitat, desc = monster
        combined_text = f"이름: {name}, 레벨: {level}, HP: {hp} 공격력: {attack}, 서식지: {habitat}, 설명: {desc}"
        embedding = generate_embedding(combined_text)

        cur.execute("update monsters set embedding = %s where id = %s",
                    (embedding, id))
        print(f"{name}임베딩 완료")

    conn.commit()

    cur.close()
    put_connection(conn)

    print("모든 몬스터 임베딩 저장 완료")

if __name__ == "__main__":
    update_monster_embeddings()
