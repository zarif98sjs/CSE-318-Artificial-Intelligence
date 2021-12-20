public class Main {

    public static void main(String[] args) {
        Board board = new Board();
        int turn = 0;

        while (true)
        {
            Agent agent0 = new Agent(2);
            agent0.setDepth(10);
            agent0.setHeuristic(1);
            Agent agent1 = new Agent(2);
            agent1.setDepth(10);
            agent1.setHeuristic(3);

            if(board.isGameOver())
            {
                System.out.println("============ Game Over ===========");
                System.out.println("==================================");

                board.printBoard();

                if(board.winner() == 0) System.out.println("Player 0 wins");
                else if(board.winner() == 1) System.out.println("Player 1 wins");
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
                board.printBoard();

                if(freeTurn) {
                    System.out.println("Free turn");
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
                board.printBoard();

                if(freeTurn) {
                    System.out.println("Free turn");
                    continue;
                }
                turn ^= 1;
            }
        }
    }
}
