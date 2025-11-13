
from openai import OpenAI

client = OpenAI()

def link_graph_nodes(nodes):
  prompt = f"""
  아래 노드 목록을 기반으로
  노드들 사이의 관계(edge)를 JSON 배열로 만들어라.
  
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
  print("link_graph_nodse 응답" + res.choices[0].message["content"])
  return res.choices[0].message["content"]