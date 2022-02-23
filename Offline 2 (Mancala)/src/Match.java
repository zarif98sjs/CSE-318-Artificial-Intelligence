import java.util.Random;

public class Match {

    public static int getRandom(int a,int b) // get a random int in [a,b]
    {
        int diff = b-a+1;
        Random rd = new Random();
        return a + (Math.abs(rd.nextInt()) % diff);
    }


    public static void main(String[] args) {

        int num_match = 50;

        int p0win = 0 , p1win = 0;

        while(num_match > 0)
        {
            Board board = new Board();
            int turn = 0;

            while (true)
            {
                Agent agent0 = new Agent(2);
//                agent0.setDepth(getRandom(1,12));
                agent0.setDepth(10);
                agent0.setHeuristic(1);
                Agent agent1 = new Agent(2);
//                agent1.setDepth(getRandom(1,12));
                agent1.setDepth(10);
                agent1.setHeuristic(1);

                if(board.isGameOver())
                {
                    System.out.println("============ Game Over ===========");
                    System.out.println("==================================");

                    board.printBoard();

                    if(board.winner() == 0) {System.out.println("Player 0 wins");p0win++;}
                    else if(board.winner() == 1) {System.out.println("Player 1 wins");p1win++;}
                    else System.out.println("Match Drawn");
                    break;
                }

                System.out.println("Turn for player "+turn+" ...");

                if(turn == 0){
                    int move = agent0.getMove(board,turn);
                    if(move == -1){
                        continue;
                    }

                    boolean freeTurn = board.move(turn,move);
//                    board.printBoard();

                    if(freeTurn) {
//                        System.out.println("Free turn");
                        continue;
                    }
                    turn ^= 1;
                }
                else{
                    int move = agent1.getMove(board,turn);
                    if(move == -1){
                        break;
                    }

                    boolean freeTurn = board.move(turn,move);
//                    board.printBoard();

                    if(freeTurn) {
//                        System.out.println("Free turn");
                        continue;
                    }
                    turn ^= 1;
                }
            }


            num_match--;
        }

        System.out.println("-------Match Summary---------");
        System.out.println("Player 0 Win "+p0win);
        System.out.println("Player 1 Win "+p1win);

    }
}
