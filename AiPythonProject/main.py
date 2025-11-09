from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel

from database.Item.Item_create_table import select_items_all
from database.Item.item_image_generate import generate_item_image
from database.monster.monster_image_generate import generate_monster_image
from database.npc.npm_chat import npc_chat_response

app = FastAPI()

app.mount("/image", StaticFiles(directory="image"), name="image")

class PlayerQuestion(BaseModel):
  question: str

class ItemIds(BaseModel):
  item_ids: list[int]

class MonsterIds(BaseModel):
  monster_ids: list[int]

@app.post("/npc/chat")
def chat_with_npc(data: PlayerQuestion):
  response = npc_chat_response(data.question)
  return {"response": response}

@app.get("/item")
def get_item_info_all():
  response = select_items_all()
  return {"response": response}

@app.post("/item/image/generate")
def generate_item_img(data:ItemIds):
  url = generate_item_image(data.item_ids)
  return {"response": "이미지 생성 완료", "ids": data.item_ids}, {"url", url}

@app.post("/monster/image/generate")
def generate_monster_img(data:MonsterIds):
  url = generate_monster_image(data.monster_ids)
  return {"response": "이미지 생성 완료", "ids": data.monster_ids}, {"url", url}