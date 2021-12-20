import java.util.Collections;
import java.util.Scanner;
import java.util.Vector;

public class Algorithm {

    private static final int INF = 100000000;

    public static ReturnNode minimax(Board board, int turn, int depth, boolean isMax,int heuristic)
    {
        if(depth == 0 || board.isGameOver()) {
            return new ReturnNode(board.Heuristic(heuristic,turn,false),-1,false);
        }

        if(isMax) {

            ReturnNode best = new ReturnNode(-INF,-1,false);

            Vector<Integer> moves = new Vector<>();
            for(int i=0;i<6;i++) moves.add(i);
            Collections.shuffle(moves);

            for(int id=0;id<6;id++)
            {
                int cur_move = moves.get(id);

                int slot_id = cur_move;
                if(turn == 1)
                {
                    slot_id = slot_id + board.slot + 1;
                }

                if(board.v[slot_id] == 0) continue;

                Board tempBoard = new Board(board);
                isMax = tempBoard.move(turn,cur_move);

                ReturnNode ret = minimax(tempBoard,turn,depth-1,isMax,heuristic);

                if(ret.bestValue > best.bestValue)
                {
                    best.bestValue = ret.bestValue;
                    best.move = cur_move;
                    best.freeMoveAvailable = isMax;
                }
            }
            return best;
        }
        else{

            ReturnNode best = new ReturnNode(INF,-1,false);

            Vector<Integer> moves = new Vector<>();
            for(int i=0;i<6;i++) moves.add(i);
            Collections.shuffle(moves);

            for(int id=0;id<6;id++)
            {
                int cur_move = moves.get(id);

                int slot_id = cur_move;
                if(turn == 1)
                {
                    slot_id = slot_id + board.slot + 1;
                }

                if(board.v[slot_id] == 0) continue;

                Board tempBoard = new Board(board);
                isMax = tempBoard.move(turn,cur_move);

                ReturnNode ret = minimax(tempBoard,turn,depth-1,!isMax,heuristic);

                if(ret.bestValue < best.bestValue)
                {
                    best.bestValue = ret.bestValue;
                    best.move = cur_move;
                    best.freeMoveAvailable = isMax;
                }
            }
            return best;
        }
    }

    public static ReturnNode minimax_alpha_beta(Board board, int turn, int depth, boolean isMax,int alpha,int beta,int heuristic)
    {
        if(depth == 0 || board.isGameOver()) {
            return new ReturnNode(board.Heuristic(heuristic,turn,false),-1,false);
        }

        if(isMax) {

            ReturnNode best = new ReturnNode(-INF,-1,false);

            Vector<Integer> moves = new Vector<>();
            for(int i=0;i<6;i++) moves.add(i);
            Collections.shuffle(moves);

            for(int id=0;id<6;id++)
            {
                int cur_move = moves.get(id);

                int slot_id = cur_move;
                if(turn == 1)
                {
                    slot_id = slot_id + board.slot + 1;
                }

                if(board.v[slot_id] == 0) continue;

                Board tempBoard = new Board(board);
                isMax = tempBoard.move(turn,cur_move);

                ReturnNode ret = minimax_alpha_beta(tempBoard,turn,depth-1,isMax,alpha,beta,heuristic);

                if(ret.bestValue > best.bestValue)
                {
                    best.bestValue = ret.bestValue;
                    best.move = cur_move;
                    best.freeMoveAvailable = isMax;
                }

                alpha = Math.max(alpha,ret.bestValue);

                if(beta <= alpha)
                    break;

            }
            return best;
        }
        else{

            ReturnNode best = new ReturnNode(INF,-1,false);

            Vector<Integer> moves = new Vector<>();
            for(int i=0;i<6;i++) moves.add(i);
            Collections.shuffle(moves);

            for(int id=0;id<6;id++)
            {
                int cur_move = moves.get(id);

                int slot_id = cur_move;
                if(turn == 1)
                {
                    slot_id = slot_id + board.slot + 1;
                }

                if(board.v[slot_id] == 0) continue;

                Board tempBoard = new Board(board);
                isMax = tempBoard.move(turn,cur_move);

                ReturnNode ret = minimax_alpha_beta(tempBoard,turn,depth-1,!isMax,alpha,beta,heuristic);

                if(ret.bestValue < best.bestValue)
                {
                    best.bestValue = ret.bestValue;
                    best.move = cur_move;
                    best.freeMoveAvailable = isMax;
                }

                beta = Math.min(beta,ret.bestValue);

                if(beta <= alpha)
                    break;

            }
            return best;
        }
    }
}
