import sys
import os
import threading

from service.monster.monster_image_generate import generate_monster_image

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..')))

import json
import psycopg2.extras
from openai import OpenAI
from db_config import get_cursor

client = OpenAI()

def get_random_world_story(bimage : bool= False):
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
    return generate_monster_from_world(world, bimage)

def generate_monster_from_world(world, bimage : bool= False):
    meta = json.loads(world['metadata']) if isinstance(world['metadata'], str) else world['metadata']
    habitat_hint = meta.get('continent', '미지의 지역')

    prompt = (
        f"아래는 한 RPG 세계관의 설정이다.\n"
        f"이 세계관에 어울리는 몬스터 1마리를 만들어줘.\n"
        f"결과는 반드시 JSON 형식으로만 출력해.\n\n"
        f"=== 세계관 제목 ===\n{world['title']}\n"
        f"=== 세계관 설명 ===\n{world['content']}\n\n"
        f"JSON 형식:\n"
        "{\n"
        "  \"name\": \"몬스터 이름\",\n"
        "  \"level\": 숫자,\n"
        "  \"hp\": 숫자,\n"
        "  \"attack\": 숫자,\n"
        f"  \"habitat\": \"서식지 (이 세계관에 어울리게, 예: {habitat_hint})\",\n"
        "  \"description\": \"이 세계관 속 분위기에 맞는 몬스터 설명 (100자이하)\"\n"
        "}"
    )

    response = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "너는 몬스터 디자이너이자 세계관 작가야."},
            {"role": "user", "content": prompt}
        ]
    )
    monster_json = json.loads(response.choices[0].message.content)
    # ✅ world_id 추가
    monster_json["world_id"] = world["id"]

    return insert_monster(monster_json, bimage)

def insert_monster(monster, bimage: bool =False):
    text_for_embedding = f"{monster['name']} {monster['description']} {monster['habitat']}"
    embedding = client.embeddings.create(
        model="text-embedding-3-small",
        input=text_for_embedding
    ).data[0].embedding

    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
        INSERT INTO monsters (name, level, hp, attack, habitat, description, embedding, world_id)
        VALUES (%s, %s, %s, %s, %s, %s, %s, %s)
        returning id;        
    """, (
        monster['name'],
        monster['level'],
        monster['hp'],
        monster['attack'],
        monster['habitat'],
        monster['description'],
        embedding,
        monster['world_id']
    ))
    monster_id = cur.fetchone()[0]
    monster['id'] = monster_id

    conn.commit()
    cur.close()
    conn.close()
    print(f"✅ 몬스터 '{monster['name']}' 생성 및 저장 완료! (world_id={monster['world_id']})")

    return [monster]

    # if bimage:
    #    threading.Thread(target=generate_monster_image, args=([monster['id']],)).start()


