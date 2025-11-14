
from openai import OpenAI

client = OpenAI()

def generate_graph_nodes(world_text: str):
    prompt = f"""
    아래 세계관 텍스트를 기반으로
    - 지역 3개
    - 세력 3개
    - 종족 2개 
    - 주요 몬스터 3개
    - 핵심 아이템 3개
      를 추출하여, 각 노드를 JSON 배열로 만들어라.

      세계관 텍스트
      {world_text}
      
      각 노드:
    {{
    "node_type": "region | faction | race | monster | item",
    "name": "...",
    "description": "..."
    }}
    """

    res = client.chat.completions.create(
        model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "세계관 그래프 생성 전문가"},
            {"role": "user", "content": prompt},
        ]
    )
    print("generate_graph_nodes 리턴값" + res.choices[0].message.content)

    return res.choices[0].message.content


