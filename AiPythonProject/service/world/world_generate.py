from openai import OpenAI
from utils.openai_client import client

client = OpenAI()


def polish_world(user_prompt: str):
    res = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
              {"role": "system", "content": "AAA RPG 세계관 작가"},
              {"role": "user", "content": user_prompt}
        ]
    )
    print("polish_world" + res.choices[0].message["content"])
    return res.choices[0].message("content")


def generate_new_world(polished: str, similar):
    s1 = similar[0]["content"] if len(similar) > 0 else ""
    s2 = similar[1]["content"] if len(similar) > 1 else ""
    s3 = similar[2]["content"] if len(similar) > 2 else ""
    prompt = f"""
    사용자 보완 세계관:
    {polished}
    
    유사 세계관 3개:
    (1) {s1}
    (2) {s2}
    (3) {s3}

    위 요소를 자연스럽게 결합해
    새로운 RPG 세계관을 만들어라
"""
    res = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "AAA RPG 세계관 작가"},
            {"role": "user", "content": prompt}
        ]
    )
    print("polish_world" + res.choices[0].message["content"])
    return res.choices[0].message("content")
