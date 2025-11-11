import base64
import requests
import os
from openai import OpenAI
import psycopg2.extras
from db_config import get_cursor

client = OpenAI()


def generate_item_image(item_ids):

    if not item_ids:
        print("아이템없음")
        return

    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("""
        select id, name, rarity, type, description
        from items
        where id = ANY(%s);
    """, (item_ids,)
    )
    rows = cur.fetchall()

    print(f"총 {len(rows)}개의 아이템 이미지 생성 시작...")

    for row in rows:
        prompt = f"""
          {row["rarity"]} 등급의 {row["type"]} 아이템 '{row["name"]}'.
          이 아이템은 {row["description"]}의 특징을 시각적으로 표현하되,
          아이템 일러스트 스타일로,
          배경은 완전히 투명하게 (PNG 느낌),
          아이템만 중앙에 정면으로,
          빛을 위쪽에서 받아 은은한 하이라이트가 생기도록,
          그림자, 텍스트, 설명문, 배경요소 없이 오직 아이템만 그려라.
        """

        try:
            translate_prompt = f"다음 아이템 이름을 영어로 자연스럽게 번역하고, 공백 대신 '_'로 바꿔줘:\n\n{row['name']}"
            name_response = client.chat.completions.create(
                model="gpt-5-nano",
                messages=[
                    {"role": "system", "content": "너는 영어 번역기야."},
                    {"role": "user", "content": translate_prompt}
                ]
            )
            translated_name = name_response.choices[0].message.content.strip()

            result = client.images.generate(
                model="gpt-image-1-mini",
                prompt=prompt,
                size="1024x1024",
                n=1
            )
            image_data = result.data[0].b64_json
            if not image_data:
                raise ValueError("❌ 이미지 데이터가 비어 있음")

            # ✅ base64 → 바이너리 변환
            image_bytes = base64.b64decode(image_data)

            # ✅ 로컬 저장 경로
            save_dir = "image/item"
            os.makedirs(save_dir, exist_ok=True)
            image_path = os.path.join(save_dir, f"{translated_name}.png")

            # ✅ 이미지 파일 저장
            with open(image_path, "wb") as f:
                f.write(image_bytes)

            print(f'[{row["id"]}] "{row["name"]}" 저장 완료 → {image_path}')

            cur.execute("""
                UPDATE items SET image_url = %s WHERE id = %s
            """, (image_path, row["id"]))
            conn.commit()

        except Exception as e:
            print(f'[{row["id"]}] "{row["name"]}" 이미지 생성 실패: {e}')
    cur.close()
    conn.close()
    print("모든 아이템 이미지 저장 완료!")
