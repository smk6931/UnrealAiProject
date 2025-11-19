from dotenv import load_dotenv
from psycopg2.pool import SimpleConnectionPool
from psycopg2.extensions import connection as PgConnection
from psycopg2.extensions import cursor as PgCursor
import psycopg2.extras

load_dotenv(interpolate=True)

# ================================================
# 🔥 타이핑 명시 → pool 자동완성 가능
# ================================================
pool: SimpleConnectionPool = SimpleConnectionPool(
    1, 20,
    host="localhost",
    port="5434",
    database="gameworld",
    user="postgres",
    password="1234"
)


# ================================================
# 🔥 get_connection() 자동완성 가능하게 타입 명시
# ================================================
def get_connection() -> PgConnection | None:
    """커넥션 풀에서 커넥션 하나 가져오기"""
    try:
        conn: PgConnection = pool.getconn()     # 타입 힌트로 conn 자동완성 OK
        return conn
    except Exception as e:
        print("커넥션 풀에서 conn 못 가져옴:", e)
        return None


# ================================================
# 🔥 put_connection() 자동완성 가능
# ================================================
def put_connection(conn: PgConnection) -> None:
    """커넥션 풀에 커넥션 반환하기"""
    try:
        pool.putconn(conn)
    except Exception as e:
        print("postgre 연결 반환 실패", e)


# ================================================
# 🔥 get_cursor() 자동완성 완벽하게 세팅
# ================================================
def get_cursor() -> tuple[PgConnection | None, PgCursor | None]:
    """커넥션 + 커서 셋업"""
    conn: PgConnection | None = get_connection()
    print("conn =", conn)

    if conn is None:
        return None, None

    # 🔥 커서 자동완성 100% 작동
    cur: PgCursor = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    return conn, cur


# def get_cursor():
#     conn = get_connection()
#     cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
#     if conn is None:
#         return None, None

#     return conn, cur


# def get_connection():
#     try:
#         conn = psycopg2.connect(
#             host="localhost",
#             port="5434",
#             database="gameworld",
#             user="postgres",
#             password="1234"
#         )
#         print("postgre 연결 성공")
#         return conn
#     except Exception as e:
#         print("postgre 연결 실패", e)
#         return None


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

# def return_connection(conn):
#     DB_POOL.putconn(conn)
