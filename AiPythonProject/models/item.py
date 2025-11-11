from typing import List
from pydantic import BaseModel


class ItemIds(BaseModel):
    item_ids: List[int]    

class MonsterId_ItemsIds(BaseModel):
    monster_id: int
    item_count: int
    bimage: bool