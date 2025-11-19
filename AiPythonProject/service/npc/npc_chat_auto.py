

from langchain_openai import ChatOpenAI

llm = ChatOpenAI(model_name="gpt-5o-mini", temperature=0)

# npc_promt = ChatPromtTemplate.from_templates(