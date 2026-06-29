namespace Data
{
    internal class Board : IBoard
    {
        public double width { get; }
        public double height { get; }

        public Board(double inWidth, double inHeight)
        {
            width = inWidth;
            height = inHeight;
        }
    }
}