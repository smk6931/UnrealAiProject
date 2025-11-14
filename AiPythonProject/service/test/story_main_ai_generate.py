from db_config import get_cursor
from openai import OpenAI
import psycopg2.extras
import json
import os
import sys

from service.world.world_embed import embed_text
sys.path.append(os.path.abspath(
    os.path.join(os.path.dirname(__file__), '../..')))

client = OpenAI()

def summarize_previous_world(question: str):
    print("summarize_previous_world 실행")

    embedding = embed_text(question)

    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cur.execute("""
        SELECT title, content, metadata,
               -(embedding <#> %s::vector) AS similarity
        FROM worlds
        ORDER BY similarity DESC
        LIMIT 1;
    """, (embedding,))
    rows = cur.fetchall()
    cur.close()
    conn.close()

    # rows가 비어 있으면 빈 리스트 반환
    if not rows:
        print("유사한 세계관 없음")
        return []

    worlds = []
    for row in rows:
        worlds.append({
            "title": row["title"],
            "content": row["content"],
            "metadata": row["metadata"],
            "similarity": row["similarity"]
        })

        print(
            "summarize_previous_world",
            "title", row["title"], "\n",
            "content", row["content"], "\n",
            "metadata", row["metadata"], "\n",
            "similarity", row["similarity"], "\n"
        )
    return worlds


def generate_next_worlds(question, quest_mood="중급"):
    print("generate_next_worlds 김치짬뽕")

    worlds = summarize_previous_world(question)

    # worlds가 비어 있으면 None
    previous_summary = json.dumps(worlds, ensure_ascii=False, indent=2) if worlds else None

    if previous_summary:
        prompt = f"""
    이전 시즌 세계관의 요약:
    {previous_summary}

    🎯 목표:
    이전 세계의 사건과 인물은 완전히 사라졌다.
    이전 시즌은 이제 신화, 고대 전설, 금단의 유물, 혹은 잊힌 기록으로만 전해진다.
    이번 시즌은 시간·공간·문명이 완전히 달라야 한다.
    이전 시즌의 직접적 요소(도시, 인물, 세력)는 절대 재등장하지 않는다.
    다만, '테마적 여운(예: 희생, 부패, 진화, 구원)'만 은유적으로 계승한다.

    🎮 새로운 시즌 설정 규칙:
    - 완전히 다른 대륙명 / 세력 구조 / 문명 배경
    - 이전 세계의 흔적은 한두 문장 정도의 신화적 언급만 허용
    - 톤 변화 필수 (예: 중세 판타지 → 스팀펑크 → 사이버펑크 → 생명공학 → 차원 전이)
    - 플레이어가 “새 시즌이 시작되었다”는 느낌을 받을 것
    - 완전히 새로운 NPC, 종족, 정치 구도, 마법 혹은 기술 체계 포함

    📜 출력은 아래 JSON 형식으로만:
    {{
    "title": "새 시즌 세계관 제목",
    "content": "이전 세계의 여운만 남고 완전히 새 문명에서 펼쳐지는 RPG 세계관 설명 (약 500자)",
    "metadata": {{
        "continent": "새로운 대륙 또는 차원 이름",
        "difficulty": "새로운 난이도",
        "quest_mood": "{quest_mood}",
        "main_factions": ["새로운 세력1", "새로운 세력2"],
        "themes": ["새로운 세계의 주제1", "주제2", "주제3"],
        "keywords": ["이전 세계에서 유산된 단어1", "새로운 단어2", "새로운 단어3"]
    }}
    }}
    """
    else:
            prompt = f"""
    이전 세계관이 존재하지 않는다.
    RPG 첫 시즌으로 완전히 새로운 세계를 만들어라.
    세계의 대륙, 주요 세력, 갈등, 문명 구조, 기술 체계를 포함하라.

    출력은 반드시 JSON 형식으로:
    {{
    "title": "세계관 제목",
    "content": "플레이어가 경험할 서사 중심의 세계관 설명 (500자 내외)",
    "metadata": {{
        "continent": "대륙 이름",
        "difficulty": "난이도",
        "quest_mood": "{quest_mood}",
        "main_factions": ["세력1", "세력2"],
        "themes": ["세계 주제1", "주제2", "주제3"],
        "keywords": ["키워드1", "키워드2", "키워드3"]
    }}
    }}
    """
    # --- GPT 호출 ---
    response = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "너는 AAA급 RPG 세계관 연출가이자 시즌 기획자다."},
            {"role": "user", "content": prompt}
        ]
    )

    world = json.loads(response.choices[0].message.content)

    # --- 임베딩 ---
    embedding_input = f"{world['title']}\n{world['content']}\n{json.dumps(world['metadata'], ensure_ascii=False)}"
    embedding = client.embeddings.create(
        model="text-embedding-3-small",
        input=embedding_input
    ).data[0].embedding

    # --- DB 저장 ---
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    cur.execute("""
        INSERT INTO worlds (title, content, metadata, embedding)
        VALUES (%s, %s, %s, %s)
    """, (
        world["title"],
        world["content"],
        json.dumps(world["metadata"]),
        embedding
    ))
    conn.commit()
    cur.close()
    conn.close()

    print(f"새 시즌 세계관 생성 완료: {world['title']}")

    return [{
        "title": world["title"],
        "content": world["content"],
        "metadata": json.dumps(world["metadata"], ensure_ascii=False)
    }]
