from typing import List

from pydantic import BaseModel


class MonsterIds(BaseModel):
    monster_ids: List[int]
