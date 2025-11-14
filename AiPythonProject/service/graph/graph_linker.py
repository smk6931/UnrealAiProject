
from openai import OpenAI

client = OpenAI()

def link_graph_nodes(nodes):
  prompt = f"""
  아래 노드 목록을 기반으로
    노드들 사이의 관계(edge)를 JSON 배열로 만들어라.

    출력 형식은 반드시 아래와 같아야 한다:
    [
      {{
        "from": "<노드 이름>",
        "to": "<노드 이름>",
        "relation": "<관계 이름>"
      }},
      ...
    ]

    절대로 "source", "target", "origin", "destination" 같은 이름을 쓰지 마라.
    무조건 "from" 과 "to" 라는 키만 사용하라.
    JSON 배열로만 출력하고 다른 말은 하지 마라.
  
  관계 예시:
  - region -> monster : "inhabits"
  - monster -> item : "drops"
  - faction -> region : "rules"
  - race -> region : "lives_in"

  노드 목록:
  {nodes}
  JSON 배열로만 출력.
  """
  
  res = client.chat.completions.create(
    model="gpt-5-mini",
        messages=[
            {"role": "system", "content": "세계관 그래프 관계 설계 전문가"},
            {"role": "user", "content": prompt}
        ]
  )
  print("link_graph_nodse 응답" + res.choices[0].message.content)
  return res.choices[0].message.content