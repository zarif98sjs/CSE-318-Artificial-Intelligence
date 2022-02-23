import java.util.Scanner;

public class Agent {
    int type;

    /**
     *
     * @param type
     * type = 0 -> HUMAN
     * type = 1 -> MINIMAX
     *
     */

    int depth;
    int heuristic;

    Agent(int type)
    {
        this.type = type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public void setDepth(int depth) {
        this.depth = depth;
    }

    public void setHeuristic(int heuristic) {
        this.heuristic = heuristic;
    }

    int getMove(Board board, int turn)
    {
        if(type == 0) { // HUMAN

            Scanner scanner = new Scanner(System.in);
            int slot_id = scanner.nextInt();
            slot_id--;

            if(!board.isValidMove(turn,slot_id)) {
                System.out.println("Invalid Move ... Try Again");
                return -1;
            }
            return slot_id;
        }
        else if(type == 1){ // MINIMAX

            ReturnNode ret = Algorithm.minimax(board,turn,depth,true,heuristic);
            System.out.println(ret);

            if(!board.isValidMove(turn,ret.move))
            {
                System.out.println("Invalid Move from AI ... Try Again");
                return -1;
            }
            return ret.move;
        }
        else if(type == 2){ // MINIMAX with pruning

            int INF = 100000000;
            ReturnNode ret = Algorithm.minimax_alpha_beta(board,turn,depth,true,-INF,INF,heuristic);
            System.out.println(ret);

            if(!board.isValidMove(turn,ret.move))
            {
                System.out.println("Invalid Move from AI ... Try Again");
                return -1;
            }
            return ret.move;
        }
        return -1;
    }
}
