from fastapi import Request, FastAPI
from fastapi.middleware.cors import CORSMiddleware
from src.python.cannon_bot_service import CannonBotService

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://ec2-54-210-171-166.compute-1.amazonaws.com:3000", 
                   "http://games-ai-1184075193.us-east-1.elb.amazonaws.com", 
                   "http://play.gamesai.co.in",
                   "http://localhost:3000"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

bot_primary_service = CannonBotService('./bin/botlib.so')
bot_secondary_service = CannonBotService('./bin/botlib.so')

@app.post("/primary/move")
async def find_best_move(request: Request):
    request_body = await request.json()
    response_move = bot_primary_service.find_best_move(request_body["gameState"], request_body["isBlackTurn"], request_body["forbiddenStates"])
    response_body = {"move": response_move}
    return response_body

@app.post("/secondary/move")
async def find_best_move(request: Request):
    request_body = await request.json()
    response_move = bot_secondary_service.find_best_move(request_body["gameState"], request_body["isBlackTurn"], request_body["forbiddenStates"])
    response_body = {"move": response_move}
    return response_body
