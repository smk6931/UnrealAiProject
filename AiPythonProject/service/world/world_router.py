from fastapi import APIRouter, Body

from service.test.story_main_ai_generate import generate_next_worlds
from service.world.world_service import generate_world_pipeline, get_random_worlds, select_worlds_all

router = APIRouter()

@router.get("/world/random")
def get_random_world():
  response = get_random_worlds()
  return {"response" : response}

@router.get("/world")
def get_world_all():
  response = select_worlds_all()
  return {"response" : response}

@router.post("/world/generate")
def generate_world_api(data: dict = Body(...)):
  question = data["question"]
  response = generate_world_pipeline(question)
  return{"response" : response}

@router.post("/world/generate/bylaststroy")
def generate_next_world(data: dict = Body(...)):
  question = data["question"]
  response = generate_next_worlds(question)
  return{"response" : response}