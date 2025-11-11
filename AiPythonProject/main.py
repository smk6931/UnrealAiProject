from typing import List

import psycopg2.extras
from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
from db_config import get_cursor
from routers import item_router, monster_router, npc_router, world_router

app = FastAPI()

app.include_router(npc_router.router)
app.include_router(monster_router.router)
app.include_router(item_router.router)
app.include_router(world_router.router)

app.mount("/image", StaticFiles(directory="image"), name="image")

@app.get("/")
def root():
  return {"message": "AI-Unreal FastAPI Server Running"}