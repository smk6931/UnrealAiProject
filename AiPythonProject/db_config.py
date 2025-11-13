from dotenv import load_dotenv
from psycopg2.pool import SimpleConnectionPool

import psycopg2.extras

load_dotenv(interpolate=True)

def get_connection():
    try:
        conn = psycopg2.connect(
            host="localhost",
            port="5434",
            database="gameworld",
            user="postgres",
            password="1234"
        )
        print("postgre 연결 성공")
        return conn
    except Exception as e:
        print("postgre 연결 실패", e)
        return None


def get_cursor():
    conn = get_connection()
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    if conn is None:
        return None, None

    return conn, cur

# 









# conn = psycopg2.connect(
        #     host="localhost",
        #     port="5434",
        #     database="gameworld",
        #     user="postgres",
        #     password="1234"
        # )

# DB_POOL = SimpleConnectionPool(
#     1, 30,
#     host="localhost",
#     port="5434",
#     dbname="gameworld",
#     user="postgres",
#     password="1234"
# )

#def return_connection(conn):
#     DB_POOL.putconn(conn)