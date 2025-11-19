import sys
import os

sys.path.append(os.path.abspath(
    os.path.join(os.path.dirname(__file__), '../..')))

import psycopg2.extras
from openai import OpenAI
from db_config import get_cursor, put_connection
import json

client = OpenAI()

def generate_worlds(continent, quest_mood="초보"):
    response = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "너는 AAA급 RPG 시나리오 디자이너이자 세계관 작가야."},
            {
                "role": "user",
                "content": (
                    f"이 세계관은 실제 RPG 게임에서 NPC, 퀘스트, 아이템, 몬스터의 근간이 될 것이므로, "
                    f"플레이어가 '직접 탐험하고 선택할 수 있는 서사' 중심으로 구성해. "
                    f"세계관의 대륙은 '{continent}'이며, 이 지역의 주요 환경과 세력, 인물, 위기 상황을 중심으로 500자 내외로 만들어줘.\n\n"
                    "출력은 반드시 아래 JSON 형식으로만:\n"
                    "{\n"
                    "  \"title\": \"세계관 제목\",\n"
                    "  \"content\": \"플레이어가 경험할 서사 중심의 세계관 설명 (500자 내외)\",\n"
                    "  \"metadata\": {\n"
                    f"      \"continent\": \"{continent}\",\n"
                    "      \"difficulty\": \"난이도\",\n"
                    f"      \"quest_mood\": \"{quest_mood}\",\n"
                    "      \"main_factions\": [\"세력1\", \"세력2\"],\n"
                    "      \"keywords\": [\"키워드1\", \"키워드2\", \"키워드3\"]\n"
                    "  }\n"
                    "}"
                )
            }
        ]
    )

    world_json = json.loads(response.choices[0].message.content)
    title = world_json["title"]
    content = world_json["content"]
    metadata = world_json["metadata"]

    embedding_input = f"{title}\n{content}\n{json.dumps(metadata, ensure_ascii=False)}"

    embedding = client.embeddings.create(
        model="text-embedding-3-small",
        input=embedding_input
    ).data[0].embedding

    conn, cur = get_cursor()
    cur.close()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
        INSERT INTO worlds (title, content, metadata, 
                embedding) VALUES (%s, %s, %s, %s)
    """, (title, content, json.dumps(metadata), embedding))
    conn.commit()
    cur.close()
    put_connection(conn)

    print(f"✅ [{continent}] 세계관 생성 완료: {title}")


continents = [
    "그라모라 평원",  # 마을 및 주변 필드
    "브린다 산맥",   # 산맥 탐험
    "노크티스 지하제국",  # 던전 중심
    "에테르 천공섬"   # 하늘 대륙
]

generate_worlds(
    "아이솔드 해협", quest_mood="초보")

# for c in continents:
#     generate_worlds(continent=c, quest_mood="초보")

