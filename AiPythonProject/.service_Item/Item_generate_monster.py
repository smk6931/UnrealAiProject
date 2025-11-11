# import sys
# import os
# from database.Item.item_image_generate import generate_item_image
# sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..')))

# from openai import OpenAI
# from db_config import get_cursor
# import json

# client = OpenAI()

# def generate_items_for_monster(monster_id: int, item_count: int = 1, bimage: bool=False):
#     conn, cur = get_cursor()

#     # 1️⃣ 몬스터 정보 조회
#     cur.execute("SELECT id, name, description, world_id FROM monsters WHERE id = %s;", (monster_id,))
#     monster = cur.fetchone()

#     if not monster:
#         print("❌ 몬스터를 찾을 수 없습니다.")
#         return

#     monster_name = monster[1]
#     monster_desc = monster[2]
#     world_id = monster[3]

#     print(f"🎯 몬스터 '{monster_name}' 기반 아이템 {item_count}개 생성 중...")

#     # 2️⃣ GPT를 이용한 아이템 생성
#     prompt = f"""
#     아래 몬스터 설명을 참고해서, RPG 게임에 어울리는 드랍 아이템 {item_count}개를 JSON 배열 형태로 만들어줘.
#     각 아이템은 'name', 'rarity', 'type', 'description' 키를 가져야 하고,
#     rarity는 ['일반', '희귀', '전설'] 중 하나로 랜덤하게 설정해.
    
#     몬스터 이름: {monster_name}
#     몬스터 설명: {monster_desc}
#     """

#     response = client.chat.completions.create(
#         model="gpt-5-mini",
#         messages=[
#             {"role": "system", "content": "너는 게임 디자이너야. 아이템 이름과 설명을 자연스럽고 RPG 스타일로 만들어라."},
#             {"role": "user", "content": prompt}
#         ]
#     )

#     items_data = []
#     try:
#         items_data = json.loads(response.choices[0].message.content)
#     except Exception as e:
#         print("❌ JSON 파싱 오류:", e)
#         print("원본 응답:", response.choices[0].message.content)
#         return

#     generate_items = []
#     generated_item_ids = []

#     # 3️⃣ 각 아이템을 DB에 저장
#     for item in items_data:
#         item_name = item.get("name")
#         rarity = item.get("rarity")
#         item_type = item.get("type")
#         desc = item.get("description")

#         # 아이템 임베딩 생성
#         embedding = client.embeddings.create(
#             model="text-embedding-3-small",
#             input=desc
#         ).data[0].embedding

#         cur.execute("""
#             INSERT INTO items (world_id, monster_id, name, rarity, type, description, embedding)
#             VALUES (%s, %s, %s, %s, %s, %s, %s)
#             RETURNING id, name, rarity, type, description, image_url;
#         """, (world_id, monster_id, item_name, rarity, item_type, desc, embedding))

#         new_item = cur.fetchone()
#         generated_item_ids.append(new_item[0])
#         generate_items.append(dict(new_item))

#     # 4️⃣ 몬스터 테이블의 drop_item_ids 업데이트
#     cur.execute("""
#         UPDATE monsters
#         SET drop_item_ids = %s
#         WHERE id = %s;
#     """, (generated_item_ids, monster_id))
    
#     conn.commit()
#     cur.close()
#     conn.close()

#     print(f"✅ '{monster_name}' 몬스터에 {len(generated_item_ids)}개의 아이템 연결 완료!")
#     print("📦 생성된 아이템 IDs:", generated_item_ids)

#     # if bimage:
#     #    generate_item_image(generated_item_ids)
#     return generate_items

