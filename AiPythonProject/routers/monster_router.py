import json
from typing import List
from fastapi import APIRouter, Body
import psycopg2.extras
from pydantic import BaseModel
from db_config import get_cursor
from models.monster import MonsterIds
from service.monster.monster_database import select_monsters, select_monsters_all
from service.monster.monster_generate import generate_monster_from_world
from service.monster.monster_image_generate import generate_monster_image

router = APIRouter()


class MonsterIds(BaseModel):
   monster_ids: List[int]


@router.get("/monster")
def get_monster_all():
  response = select_monsters_all()
  return {"response": response}


@router.post("/monster/image/generate")
def generate_monster_img(data: MonsterIds):
  url = generate_monster_image(data.monster_ids)
  return {"response": "이미지 생성 완료", "ids": data.monster_ids}, {"url", url}

# @router.post("/monster/generate")
# def get_generate_monster_fullinfo():
#   response = get_random_world_story()
#   return {"response": response}


@router.post("/monster/generate")
def get_generate_monster_world(data: dict = Body(...)):
  if "response" in data:
    data["world"] = data.pop("response")

    world = data["world"][0]
    response = generate_monster_from_world(world)
    return {"response": response}

  # world = json.loads(world)
  # response = generate_monster_from_world(world)
  # return {"response" : response}

@router.post("/monster/get/monster_ids")
def get_monsters(data:MonsterIds):
  response = select_monsters(data.monster_ids)
  return{"response": response}