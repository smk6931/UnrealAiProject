import os
import sys
sys.path.append(os.path.abspath(
os.path.join(os.path.dirname(__file__), '../../')))

from db_config import get_cursor
from openai import OpenAI
import psycopg2
import psycopg2.extras

client = OpenAI()

def get_similar_world_stories(question, top_n=1):
    question_embedding = client.embeddings.create(
        model="text-embedding-3-small",
        input=question
    ).data[0].embedding

    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
        SELECT id, title, content, metadata,
               -(embedding <#> %s::vector) AS similarity
        FROM worlds
        ORDER BY embedding <#> %s::vector
        LIMIT %s;
    """, (question_embedding, question_embedding, top_n))
    results = cur.fetchall()

    for result in results:
        print("📘 스토리:", result["title"])
        print("   유사도(distance):", result["similarity"])
        print("   요약:", result["content"][:150], "...\n")

    cur.close()
    conn.close()

    return results

def npc_chat_response(player_question):

    print("💬유저 질문??:",  player_question)
    similar_stories = get_similar_world_stories(player_question)

    context_text = ""
    for story in similar_stories:
        meta = story["metadata"]
        context_text += (
            f"[{story['title']}]\n"
            f"{story['content']}\n"
            f"continent: {meta.get('continent', 'unknown')}, "
        )
    prompt = f"""
        너는 RPG 게임 속 NPC다.

        플레이어의 질문을 먼저 분석하여 아래 두 가지 중 하나로 분류하라.
        1) 세계관에 대한 질문인지?
        2) 세계관이 아닌, 플레이/전투/인물/진행 방향 등에 대한 질문인지?

        분류 결과에 따라 답변 방식을 다르게 한다.

        ---------------------------------------
        [규칙 A — 세계관 질문일 때]
        - 임베딩으로 찾은 세계관(context_text)의 내용을 기반으로 답한다.
        - 필요한 만큼 친절하게 설명해도 된다.
        - 그러나 “설명체”가 아니라 NPC가 알고 있는 이야기처럼 대사 형태로 말한다.
        - 최대 5줄.

        [규칙 B — 세계관 질문이 아닐 때]
        - 세계관을 직접 설명하지 않는다.
        - 대신 세계관의 분위기·문화·사건·정서를 은유적으로 녹여서,
        NPC 시점으로 자연스럽게 답한다.
        - “이 세계는 ~이다” 같은 설명체는 금지.
        - 최대 5줄.

        ---------------------------------------
        [참고용 세계관 정보(직접 언급 금지)]
        {context_text}

        [플레이어 질문]
        {player_question}

        ---------------------------------------
        NPC의 대답:
        """
    response = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "너는 감정이 있고 배경지식을 가진 NPC야."},
            {"role": "user", "content": prompt}
        ]
    )
    # print("📘 스토리:", result["title"])
    # print("   유사도(distance):", result["similarity"])

    npc_reply = response.choices[0].message.content.strip()
    print("💬NPC 답변은??:",  npc_reply)

    return {
             "reply" : npc_reply,
             "title" : similar_stories[0]["title"],
             "similarity" : similar_stories[0]["similarity"]
           }