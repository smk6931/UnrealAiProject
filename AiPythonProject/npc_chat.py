from langchain.embeddings import OpenAIEmbeddings
from langchain.vectorstores import FAISS
from langchain.text_splitter import CharacterTextSplitter
from langchain.chat_models import ChatOpenAI
from langchain.memory import ConversationBufferMemory
from langchain.chains import ConversationalRetrievalChain
from dotenv import load_dotenv
import os

# Load environment variables
load_dotenv()


class NPCChatSystem:
    def __init__(self):
        self.embeddings = OpenAIEmbeddings()
        self.vector_store = None
        self.chat = None
        self.setup_chat_system()

    def setup_chat_system(self):
        # Initialize the chat model
        llm = ChatOpenAI(temperature=0.7)

        # Initialize memory
        memory = ConversationBufferMemory(
            memory_key="chat_history",
            return_messages=True
        )

        # Load and process the knowledge base
        self.load_knowledge_base()

        # Create the conversational chain
        self.chat = ConversationalRetrievalChain.from_llm(
            llm=llm,
            retriever=self.vector_store.as_retriever(),
            memory=memory,
        )

    def load_knowledge_base(self):
        # Load the NPC's knowledge base
        try:
            with open("data/npc_knowledge.txt", "r", encoding="utf-8") as f:
                raw_text = f.read()

            # Split text into chunks
            text_splitter = CharacterTextSplitter(
                separator="\n",
                chunk_size=1000,
                chunk_overlap=200,
                length_function=len
            )
            texts = text_splitter.split_text(raw_text)

            # Create vector store
            self.vector_store = FAISS.from_texts(texts, self.embeddings)
            print("Knowledge base loaded successfully!")
        except Exception as e:
            print(f"Error loading knowledge base: {e}")

    def chat_with_npc(self, user_input):
        if not self.chat:
            return "Chat system not initialized properly."

        try:
            response = self.chat({"question": user_input})
            return response['answer']
        except Exception as e:
            return f"Error processing response: {e}"


def main():
    # Initialize the NPC chat system
    npc = NPCChatSystem()

    print("NPC Chat System initialized. Type 'quit' to exit.")

    while True:
        user_input = input("You: ")
        if user_input.lower() == 'quit':
            break

        response = npc.chat_with_npc(user_input)
        print(f"NPC: {response}")


if __name__ == "__main__":
    main()
