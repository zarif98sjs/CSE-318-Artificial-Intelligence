public class ReturnNode {
    int bestValue;
    int move;
    boolean freeMoveAvailable;

    ReturnNode(int heuristicValue,int move,boolean freeMoveAvailable)
    {
        this.bestValue = heuristicValue;
        this.move = move;
        this.freeMoveAvailable = freeMoveAvailable;
    }

    ReturnNode(ReturnNode returnNode)
    {
        bestValue = returnNode.bestValue;
        move = returnNode.move;
        freeMoveAvailable = returnNode.freeMoveAvailable;
    }

    @Override
    public String toString() {
        return "ReturnNode{" +
                "bestValue=" + bestValue +
                ", move=" + (move+1) +
                '}';
    }
}
