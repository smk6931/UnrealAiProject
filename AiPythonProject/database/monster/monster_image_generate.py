
import base64
import os
from openai import OpenAI
import psycopg2.extras

from db_config import get_cursor


client = OpenAI()


def generate_monster_image(monster_ids):
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
      select id, name, habitat, description
      from monsters
      where id = any(%s);
  """, (monster_ids,))
    rows = cur.fetchall()

    print(f"총 {len(rows)}개의 몬스터 이미지 생성 시작...")

    os.makedirs("image/monster", exist_ok=True)

    for row in rows:
        prompt = f"""
    몬스터 이름 : '{row["name"]}'
    서식지: {row["habitat"]}
    설명 : {row["description"]}
    
    위 정보를 참고해서, RPG 게임용 몬스터 일러스트를 만들어라.
    몬스터의 외형적 특징과 분위기를 시각적으로 표현하되,
    **게임 일러스트 스타일로**,
    배경은 완전히 투명하게 (PNG 느낌),
    몬스터만 중앙 정면에서 보이게 하며,
    빛은 위쪽에서 받아 은은한 하이라이트가 생기도록.
    **그림자, 텍스트, 장식, 배경요소 없이 오직 몬스터만** 그려라.
    사실적 질감, 섬세한 조명, 고품질, 판타지 스타일.        
    """
        try:
          translate_prompt = f"다음 몬스터 이름을 영어로 자연스럽게 번역하고, 공백 대신 '_'로 바꿔줘:\n\n{row['name']}"
          name_response = client.chat.completions.create(
            model="gpt-5-nano",
            messages=[
                {"role": "system", "content": "너는 영어 번역기야."},
                {"role": "user", "content": translate_prompt}
            ]
          )
          translate_name = name_response.choices[0].message.content.strip()

          result = client.images.generate(
            model="gpt-image-1-mini",
            prompt=prompt,
            size="1024x1024",
            n=1
          )

          image_data = result.data[0].b64_json
          if not image_data:
            raise ValueError("이미지 데이터가 비어 있음")

          image_bytes = base64.b64decode(image_data)

          image_path = os.path.join("image/monster", f"{translate_name}.png")

          with open(image_path, "wb") as f:
            f.write(image_bytes)
          print(f'[{row["id"]}] "{row["name"]}" 저장 완료 → {image_path}')

          cur.execute("""
            update monsters set image_url = %s where id = %s
          """, (image_path, row["id"]))
          conn.commit()

        except Exception as e:
          print(f'[{row["id"]}] "{row["name"]}" 이미지 생성 실패 : {e}')

    cur.close()
    conn.close()
    print("✅ 모든 몬스터 이미지 저장 완료!")
