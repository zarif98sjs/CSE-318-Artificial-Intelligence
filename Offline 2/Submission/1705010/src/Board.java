import java.util.Vector;

public class Board {

    int v[];
    int p0Mancala = 6;
    int p1Mancala = 13;
    int slot = 6;
    int stone = 4;
    int boardSize = 2*slot + 2;

    // p1 has 6 slot and 1 mancala
    // p2 has 6 slot and 1 mancala

    Board()
    {
        v = new int[boardSize];

        int st1 = 0;
        for(int i=st1;i<st1+slot;i++) v[i] = stone;
        v[st1+slot] = 0;

        int st2 = slot+1;
        for(int i=st2;i<st2+slot;i++) v[i] = stone;
        v[st2+slot] = 0;

        printBoard();
    }

    public Board(Board board)
    {
//        System.out.println("Copy Constructor");
        v = board.v.clone();
        p0Mancala = board.p0Mancala;
        p1Mancala = board.p1Mancala;
        slot = board.slot;
        stone = board.stone;
        boardSize = board.boardSize;
    }

    void printBoard()
    {
//        for(int i=0;i< v.length;i++) System.out.print(v[i]+" ");
//        System.out.println("");

        int st2 = slot+1;
        System.out.print("\t\t");
        for(int i=st2+slot-1;i>=st2;i--) System.out.print(v[i]+" ");
        System.out.println("");

        System.out.println(v[p1Mancala]+"\t\t\t\t\t\t\t"+v[p0Mancala]);

        int st1 = 0;
        System.out.print("\t\t");
        for(int i=st1;i<st1+slot;i++) System.out.print(v[i]+" ");
        System.out.println("");
    }

    boolean move(int turn,int slot_id) // slot_id is 0 based
    {
        if(turn == 0)
        {
            // turn for player 0

            int num_move = v[slot_id];

            int now = slot_id+1;
            while(num_move > 0)
            {
                if(now == p1Mancala)
                {
                    now = (now+1)%boardSize;
                    continue;
                }

                v[now]++;
                v[slot_id]--;
                num_move--;

                if(num_move == 0) // last move
                {
                    if(now == p0Mancala) return true; // free move

                    int opposite_slot_id = Math.abs(2*slot - now);
                    if(v[now] == 1 && now >= 0 && now <= 5 && v[opposite_slot_id] > 0) // capture
                    {
                        v[p0Mancala] += v[opposite_slot_id];
                        v[p0Mancala] += v[now];
                        v[opposite_slot_id] = 0;
                        v[now] = 0;
//                        System.out.println("Opposite "+opposite_slot_id);
//                        System.out.println("Capture");
                    }
                }

                now = (now+1)%boardSize;
            }


        }
        else{

            // turn for player 1

            slot_id = slot_id + slot + 1;
            int num_move = v[slot_id];

            int now = slot_id+1;
            while(num_move > 0)
            {
                if(now == p0Mancala)
                {
                    now = (now+1)%boardSize;
                    continue;
                }

                v[now]++;
                v[slot_id]--;
                num_move--;

                if(num_move == 0) // last move
                {
                    if(now == p1Mancala) return true; // free move

                    int opposite_slot_id = Math.abs(2*slot - now);
                    if(v[now] == 1 && now >= 7 && now <= 12 && v[opposite_slot_id] > 0) // capture
                    {
                        v[p1Mancala] += v[opposite_slot_id];
                        v[p1Mancala] += v[now];
                        v[opposite_slot_id] = 0;
                        v[now] = 0;
//                        System.out.println("Opposite "+opposite_slot_id);
//                        System.out.println("Capture");
                    }
                }

                now = (now+1)%boardSize;
            }
        }
        return false;
    }

    boolean isGameOver()
    {
        int st1 = 0;
        int cnt = 0;
        for(int i=st1;i<st1+slot;i++){
            cnt += v[i];
        }

        if(cnt == 0) {

            // player 0 has no more move
            // player 1 will get the remaining stones

            int st2 = slot+1;
            for(int i=st2;i<st2+slot;i++){
                v[p1Mancala] += v[i];
                v[i] = 0;
            }

            return true;
        }

        int st2 = slot+1;
        cnt = 0;
        for(int i=st2;i<st2+slot;i++){
            cnt += v[i];
        }

        if(cnt == 0){
            st1 = 0;
            for(int i=st1;i<st1+slot;i++){
                v[p0Mancala] += v[i];
                v[i] = 0;
            }
            return true;
        }

        return false;
    }

    boolean isValidMove(int turn,int slot_id)
    {
        if(slot_id < 0 || slot_id > 5) return false;

        if(turn == 1) slot_id = slot_id + slot + 1;

        if(v[slot_id] <= 0) return  false;
        return true;
    }

    int winner()
    {
        if(v[p0Mancala] > v[p1Mancala]) return 0;
        else if (v[p1Mancala] > v[p0Mancala]) return 1;
        return 2; // draw
    }

    int getP0Score() {
        return v[p0Mancala];
    }

    int getP1Score() {
        return v[p1Mancala];
    }

    int getP0Stones() {
        int st1 = 0,cnt = 0;
        for(int i=st1;i<st1+slot;i++){
             cnt += v[i];
        }
        return cnt;
    }

    int getP1Stones() {
        int st2 = slot+1,cnt = 0;
        for(int i=st2;i<st2+slot;i++){
            cnt += v[i];
        }
        return cnt;
    }

    int getP0StonesCloseToStorage(){
        int st1 = 0,cnt = 0;
        for(int i=st1;i<st1+slot;i++){
            if(v[i] > (p0Mancala - i)){
                cnt += v[i] - (p0Mancala - i);
            }
        }
        return cnt;
    }

    int getP1StonesCloseToStorage(){
        int st2 = slot+1,cnt = 0;
        for(int i=st2;i<st2+slot;i++){
            if(v[i] > (p1Mancala - i)){
                cnt += v[i] - (p1Mancala - i);
            }
        }
        return cnt;
    }

    int getP0StonesWeighted(){
        int st1 = 0,cnt = 0;
        for(int i=st1;i<st1+slot;i++){
                cnt += (p0Mancala - i) * v[i];
        }
        return cnt;
    }

    int getP1StonesWeighted(){
        int st2 = slot+1,cnt = 0;
        for(int i=st2;i<st2+slot;i++){
                cnt += (p1Mancala - i) * v[i];
        }
        return cnt;
    }



    int h1(int turn)
    {
        if(turn == 0) return getP0Score() - getP1Score();
        else return getP1Score()- getP0Score();
    }

    int h2(int turn)
    {
        int W1 = 4 , W2 = 6;
        if(turn == 0){
            return W1 * (getP0Score() - getP1Score()) + W2 * (getP0Stones() - getP1Stones());
        }
        else{
            return W1 * (getP1Score() - getP0Score()) + W2 * (getP1Stones() - getP0Stones());
        }
    }

    int h3(int turn,boolean freeMoveAvailable)
    {
        int W1 = 4 , W2 = 2, W3 = 4;
        int free = freeMoveAvailable ? 1 : 0;
        if(turn == 0){
            return W1 * (getP0Score() - getP1Score()) + W2 * (getP0Stones() - getP1Stones()) + W3 * free;
        }
        else{
            return W1 * (getP1Score() - getP0Score()) + W2 * (getP1Stones() - getP0Stones()) + W3 * free;
        }
    }

    int h4(int turn)
    {
        if(turn == 0) return getP0StonesCloseToStorage() - getP1StonesCloseToStorage();
        else return getP1StonesCloseToStorage()- getP0StonesCloseToStorage();
    }

    int h5(int turn)
    {
        int W1 = 4 , W2 = 6;
        if(turn == 0){
            return W1 * (getP0Score() - getP1Score()) + W2 * (getP0StonesCloseToStorage() - getP1StonesCloseToStorage());
        }
        else{
            return W1 * (getP1Score() - getP0Score()) + W2 * (getP1StonesCloseToStorage() - getP0StonesCloseToStorage());
        }
    }

    int h6(int turn,boolean freeMoveAvailable)
    {
        int W1 = 4 , W2 = 2, W3 = 4;
        int free = freeMoveAvailable ? 1 : 0;
        if(turn == 0){
            return W1 * (getP0Score() - getP1Score()) + W2 * (getP0StonesCloseToStorage() - getP1StonesCloseToStorage()) + W3 * free;
        }
        else{
            return W1 * (getP1Score() - getP0Score()) + W2 * (getP1StonesCloseToStorage() - getP0StonesCloseToStorage()) + W3 * free;
        }
    }

    int h7(int turn)
    {
        if(turn == 0) return getP0StonesWeighted() - getP1StonesWeighted();
        else return getP1StonesWeighted()- getP0StonesWeighted();
    }

    int Heuristic(int h,int turn,boolean freeMoveAvailable){
        if(h == 1) return h1(turn);
        else if (h == 2) return h2(turn);
        else if (h == 3) return h3(turn,freeMoveAvailable);
        else if (h == 4) return h4(turn);
        else if (h == 5) return h5(turn);
        else if (h == 6) return h6(turn,freeMoveAvailable);
        else if (h == 7) return h7(turn);
        return 0;
    }
}
