from fastapi import APIRouter
from pydantic import BaseModel
from service.npc.npc_chat import npc_chat_response

router = APIRouter()

class PlayerQuestion(BaseModel):
    question: str

@router.post("/npc/chat")
def chat_with_npc(data: PlayerQuestion):
  response = npc_chat_response(data.question)
  return {"response": response}
