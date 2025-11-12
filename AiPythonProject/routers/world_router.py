from fastapi import APIRouter

from service.story.world_database import get_random_worlds, select_worlds_all

router = APIRouter()

@router.get("/world/random")
def get_random_world():
  response = get_random_worlds()
  return {"response" : response}

@router.get("/world")
def get_world_all():
  response = select_worlds_all()
  return {"response" : response}