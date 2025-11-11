# from typing import List
# from fastapi import FastAPI
# from fastapi.staticfiles import StaticFiles
# import psycopg2.extras
# from pydantic import BaseModel

# from database.Item.Item_create_table import select_items_all
# from database.Item.Item_generate_monster import generate_items_for_monster
# from database.Item.item_image_generate import generate_item_image
# from database.monster.monster_createtable import select_monsters_all
# from database.monster.monster_generate import get_random_world_story
# from database.monster.monster_image_generate import generate_monster_image
# from database.npc.npm_chat import npc_chat_response
# from db_config import get_cursor
# from routers import monster_router, npc_router

# app = FastAPI()

# app.include_router(npc_router.router)
# app.include_router(monster_router)

# app.mount("/image", StaticFiles(directory="image"), name="image")

# class PlayerQuestion(BaseModel):
#   question: str

# class ItemIds(BaseModel):
#   item_ids: list[int]

# class MonsterId_ItemsIds(BaseModel):
#   monster_id: int
#   item_count: int
#   bimage: bool = False

# class MonsterIds(BaseModel):
#   monster_ids: list[int]

# @app.post("/npc/chat")
# def chat_with_npc(data: PlayerQuestion):
#   response = npc_chat_response(data.question)
#   return {"response": response}



# @app.get("/item")
# def get_item_info_all():
#   response = select_items_all()
#   return {"response": response}

# @app.post("/item/image/generate")
# def generate_item_img(data:ItemIds):
#   url = generate_item_image(data.item_ids)
#   return {"response": "이미지 생성 완료", "ids": data.item_ids}, {"url", url}

# @app.post("/item/generate/monster_ids")
# def generate_items_monsterIds(data: MonsterId_ItemsIds):
#   response = generate_items_for_monster(
#                        monster_id = data.monster_id,
#                        item_count = data.item_count,
#                        bimage = data.bimage)
#   return {"response": response}


# @app.get("/item/monster/{id}")
# def get_item_image_status(monster_id: int):
#     conn, cur = get_cursor()
#     cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
#     cur.execute("SELECT image_url FROM items WHERE id = %s;", (id,))
#     result = cur.fetchone()
#     cur.close()
#     conn.close()
#     if not result or not result["image_url"]:
#         return {"status": "processing"}
#     return {"status": "done", "image_url": result["image_url"]}

# @app.get("/monster")
# def get_monster_info_all():
#   response = select_monsters_all()
#   return {"response": response}

# @app.post("/monster/image/generate")
# def generate_monster_img(data:MonsterIds):
#   url = generate_monster_image(data.monster_ids)
#   return {"response": "이미지 생성 완료", "ids": data.monster_ids}, {"url", url}

# @app.post("/monster/generate")
# def get_generate_monster_fullinfo():
#   response = get_random_world_story()
#   return {"response": response}

# @app.get("/monster/image/{monster_id}")
# def get_monster_image_status(monster_id: int):
#     conn, cur = get_cursor()
#     cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

#     cur.execute("SELECT image_url FROM monsters WHERE id = %s;", (monster_id,))
#     result = cur.fetchone()

#     cur.close()
#     conn.close()

#     if not result or not result["image_url"]:
#         return {"status": "processing"}
#     return {"status": "done", "image_url": result["image_url"]}
