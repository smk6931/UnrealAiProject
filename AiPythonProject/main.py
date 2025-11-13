from typing import List

import psycopg2.extras
from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
from db_config import get_cursor
from service.story import world_router
from service.Item import item_router
from service.monster import monster_router
from service.npc import npc_router

app = FastAPI()

app.include_router(npc_router.router)
app.include_router(monster_router.router)
app.include_router(item_router.router)
app.include_router(world_router.router)

app.mount("/image", StaticFiles(directory="image"), name="image")

@app.get("/")
def root():
  return {"message": "AI-Unreal FastAPI Server Running"}