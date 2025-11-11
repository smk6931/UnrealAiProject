from fastapi import APIRouter
import psycopg2.extras
from db_config import get_cursor
from models.monster import MonsterIds
from service.monster.monster_createtable import select_monsters_all
from service.monster.monster_generate import get_random_world_story
from service.monster.monster_image_generate import generate_monster_image

router = APIRouter()

@router.get("/monster")
def get_monster_info_all():
  response = select_monsters_all()
  return {"response": response}

@router.post("/monster/image/generate")
def generate_monster_img(data:MonsterIds):
  url = generate_monster_image(data.monster_ids)
  return {"response": "이미지 생성 완료", "ids": data.monster_ids}, {"url", url}

@router.post("/monster/generate")
def get_generate_monster_fullinfo():
  response = get_random_world_story()
  return {"response": response}

@router.get("/monster/image/{monster_id}")
def get_monster_image_status(monster_id: int):
    conn, cur = get_cursor()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cur.execute("SELECT image_url FROM monsters WHERE id = %s;", (monster_id,))
    result = cur.fetchone()

    cur.close()
    conn.close()

    if not result or not result["image_url"]:
        return {"status": "processing"}
    return {"status": "done", "image_url": result["image_url"]}
