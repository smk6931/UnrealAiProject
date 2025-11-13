from typing import List
from fastapi import APIRouter
from pydantic import BaseModel
from service.Item.Item_database import select_items, select_items_all
from service.Item.Item_generate_monster import generate_items_for_monster
from service.Item.item_image_generate import generate_item_image

router = APIRouter()

class ItemIds(BaseModel):
    item_ids: List[int]    

class MonsterId_ItemsIds(BaseModel):
    monster_id: int
    item_count: int
    bimage: bool

@router.get("/item")
def get_item_info_all():
    response = select_items_all()
    return {"response": response}

@router.post("/item/image/generate")
def generate_item_img(data: ItemIds):
    url = generate_item_image(data.item_ids)
    return {"response": "이미지 생성 완료", "ids": data.item_ids}, {"url", url}

@router.post("/item/generate/monster_ids")
def generate_items_monsterIds(data: MonsterId_ItemsIds):
    response = generate_items_for_monster(
        monster_id=data.monster_id,
        item_count=data.item_count,
        bimage=data.bimage)
    return {"response": response}

@router.post("/item/get/item_ids")
def get_items(data: ItemIds):
    response = select_items(data.item_ids)
    return {"response": response}