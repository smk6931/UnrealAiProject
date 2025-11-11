from fastapi import APIRouter

from service.story.world_database import get_random_world_story

router = APIRouter()

@router.get("/world/random")
def get_random_world():
  response = get_random_world_story()
  return {"response" : response}