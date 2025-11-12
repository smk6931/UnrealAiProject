import json
from db_config import get_cursor
from openai import OpenAI
import psycopg2.extras
import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

client = OpenAI()


def generate_worlds(quest_mood="초보"):

    conn, cur = get_cursor()
    cur.close()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
        select title, content, metadata
                from worlds
                order by id desc
                limit 1
        """)
    last_world = cur.fetchone()
    if last_world is None:
       previous_world = "이전 세계관 데이터가 없습니다. 완전히 새로운 세계관을 생성하세요."
    else:
       previous_world = f"""
        이전 세계관: 
        제목: {last_world['title']}
        내용: {last_world['content']}
        메타데이터: {last_world['metadata']}
    """

    response = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "너는 세계관 작가이자 RPG 게임 시나리오 디자이너야."},
            {
                "role": "user",
                "content": (
                    previous_world +
                    "\n\n이전 세계관의 주요 설정을 일부 계승하면서, 새로운 갈등, 세력, 인물, 기술 체계를 추가해 "
                    "다음 게임용 세계관을 만들어줘. "
                    "이 세계관은 실제 RPG 게임에서 NPC, 퀘스트, 아이템, 몬스터의 근간이 될 것이므로, "
                    "플레이어가 '직접 탐험하고 선택할 수 있는 서사' 중심으로 구성해. "
                    "역사서처럼 설명하지 말고, 서사의 현재 시점에서 세계가 어떤 위기나 사건을 맞고 있는지를 보여줘. "
                    f"전체적인 분위기는 '{quest_mood}' 퀘스트의 감정선을 따라가되, "
                    "게임 스토리로 확장하기 쉽게 세력, 인물, 지역 간 갈등을 분명히 해줘.\n\n"
                    "출력은 반드시 아래 JSON 형식으로만.\n\n"
                    "{\n"
                    "  \"title\": \"세계관 제목\",\n"
                    "  \"content\": \"플레이어가 경험할 서사 중심의 세계관 설명 (500자 내외)\",\n"
                    "  \"metadata\": {\n"
                    "      \"continent\": \"새로운 또는 기존 대륙 이름\",\n"
                    "      \"difficulty\": \"난이도\",\n"
                    f"     \"quest_mood\": \"{quest_mood}\",\n"
                    "      \"main_factions\": [\"세력1\", \"세력2\"],\n"
                    "      \"keywords\": [\"키워드1\", \"키워드2\", \"키워드3\"]\n"
                    "  }\n"
                    "}"
                )
            }
        ]
    )

    import json
    world_json = json.loads(response.choices[0].message.content)

    title = world_json["title"]
    content = world_json["content"]
    metadata = world_json["metadata"]

    embedding_input = f"{title}\n{content}\n{json.dumps(metadata, ensure_ascii=False)}"

    embedding = client.embeddings.create(
        model="text-embedding-3-small",
        input=embedding_input
    ).data[0].embedding

    print("response ", response.choices[0].message.content)
    print("json변환 ", world_json)

    cur.execute("""
            insert into worlds (title, content, metadata, embedding)
            values (%s, %s, %s, %s)
            """, (title, content, json.dumps(metadata), embedding))
    conn.commit()
    cur.close()
    conn.close()
