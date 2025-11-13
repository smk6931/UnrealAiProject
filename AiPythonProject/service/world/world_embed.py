from utils.openai_client import client

def embed_text(text: str):
  res = client.embeddings.create(
    model="text-embedding-3-small",
    input=text
  )
  return res.data[0].embedding