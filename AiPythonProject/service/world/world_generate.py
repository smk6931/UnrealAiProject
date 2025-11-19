import json
from openai import OpenAI
client = OpenAI()

# RangChain 초기 실패작

def polish_world(user_prompt: str):

    prompt = f"""
    아래 세계관 아이디어를 RPG 세계관 작가의 시점에서
    200~300자 이내로 핵심만 깔끔하게 요약해라. 
    핵심 지역, 분위기, 세계관 정체성만 포함해라.
    {user_prompt}
    """

    res = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
              {"role": "system", "content": "AAA RPG 세계관 작가"},
              {"role": "user", "content": prompt}
        ]
    )
    content = res.choices[0].message.content
    print("polish_world 시작 : " + content)
    return content

def generate_new_world(polished, similar):
    s1 = similar[0]["content"] if len(similar) > 0 else ""
    s2 = similar[1]["content"] if len(similar) > 1 else ""
    s3 = similar[2]["content"] if len(similar) > 2 else ""

    prompt = f"""
    사용자 보완 세계관 요약:
    {polished}

    참고용 비슷한 세계관 3개:
    1) {s1}
    2) {s2}
    3) {s3}

    요구사항:
    - 완전히 새로운 대륙이나 독창적인 판타지 세계관을 만든다.
    - 기존 세계관 요소는 ‘참고’만 하고 직접 복사 금지.
    - 지형·문화·생태·마법·기술 등을 자유롭게 창작하되,
      앞으로 몬스터·아이템·퀘스트 확장이 가능하도록 핵심 테마와 분위기를 함께 설계한다.
    - 전체 분량은 600~900자 내외로 제한한다.
    - 제목(title)은 세계관 핵심을 담은 “12자 내외” 짧은 제목으로 생성할 것.
    - content 세계관 핵심을 3~4줄 요약으로 구성한다.
    - details는 지역·세력·생태·갈등 구조 등 확장 가능한 요소를 자연스럽게 포함한다.

    출력 형식(JSON):
    {{
      "title": "12자 내외",
      "content": "...",
      "details": "..."
    }}
    """

    res = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "창의적 판타지 세계관 전문 작가"},
            {"role": "user", "content": prompt}
        ]
    )
    world = json.loads(res.choices[0].message.content)
    print("generate_new_world 생성 세계관" + "\n" + 
          "세계관 content" + world["content"])
    
    return world
