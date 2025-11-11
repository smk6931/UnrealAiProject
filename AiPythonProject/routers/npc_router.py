from fastapi import APIRouter
from models.schemas import PlayerQuestion
from service.npc.npm_chat import npc_chat_response

router = APIRouter()

@router.post("/npc/chat")
def chat_with_npc(data: PlayerQuestion):
  response = npc_chat_response(data.question)
  return {"response": response}
