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
            # f"difficulty: {meta.get('difficulty', 'unknown')}, "
            # f"keywords: {meta.get('keywords', [])}\n"
        )
    prompt = (
        f"너는 RPG 게임의 NPC다. 플레이어의 질문에 대해 짧게 대답해라.\n"
        f"답변은 반드시 5줄 이내로 하고, 감정은 자연스럽고 과하지 않게.\n"
        f"세계관 요약은 말하지 말고, 직접 대화하듯 말해라.\n\n"
        f"=== 세계관 정보 ===\n{context_text}\n"
        f"=== 플레이어 질문 ===\n{player_question}\n\n"
        f"=== NPC의 답변 ==="
    )

    response = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "너는 감정이 있고 배경지식을 가진 NPC야."},
            {"role": "user", "content": prompt}
        ]
    )
    npc_reply = response.choices[0].message.content.strip()
    print("💬NPC 답변은??:",  npc_reply)

    return npc_reply


if __name__ == "__main__":
    npc_answer = npc_chat_response("에테르를 얻을 수 있는곳은?")
