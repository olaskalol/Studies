using Data;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace Logic
{
    internal class LogicAPI : LogicAbstractApi
    {
        private readonly DataAbstractApi dataApi;
        private readonly IBoard board;
        private List<IBall> balls;
        private Random random = new Random();
        private CancellationTokenSource cancelTokenSource;
        private readonly object _lock = new object();
        public override event EventHandler PositionChanged;
        public LogicAPI(DataAbstractApi inDataApi)
        {
            dataApi = inDataApi;
            board = inDataApi.GetBoard();
            balls = new List<IBall>();
        }

        public override IEnumerable<IBall> GetBalls() => balls;

        public override void StartSimulation(int numberOfBalls)
        {
            if (cancelTokenSource != null && !cancelTokenSource.IsCancellationRequested)
                return;

            balls.Clear();

            double radius = 15;
            double weight = 15;

            for (int i = 0; i < numberOfBalls; i++)
            {
                double px, py;
                bool isOverlapping;

                do
                {
                    isOverlapping = false;
                    px = random.NextDouble() * (board.width - 2 * radius) + radius;
                    py = random.NextDouble() * (board.height - 2 * radius) + radius;

                    foreach (var b in balls)
                    {
                        double distance = Math.Sqrt(Math.Pow(px - b.position.X, 2) + Math.Pow(py - b.position.Y, 2));
                        if (distance < (radius + b.radius))
                        {
                            isOverlapping = true;
                            break;
                        }
                    }
                } while (isOverlapping);

                double vx = (random.NextDouble() * 4) - 2;
                double vy = (random.NextDouble() * 4) - 2;

                IBall newBall = DataAbstractApi.CreateBall(px, py, vx, vy, radius, weight);
                balls.Add(newBall);
            }

            cancelTokenSource = new CancellationTokenSource();
            foreach (var ball in balls)
            {
                ball.StartMoving(cancelTokenSource.Token);
            }
            Task.Run(() => CollisionMonitorLoop(cancelTokenSource.Token));
        }

        public override void StopSimulation() => cancelTokenSource?.Cancel();

        private async Task CollisionMonitorLoop(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                HandleWallCollisions();
                HandleBallCollisions();

                PositionChanged?.Invoke(this, EventArgs.Empty);
                await Task.Delay(5, token);
            }
        }

        private void HandleBallCollisions()
        {
            for (int i = 0; i < balls.Count; i++)
            {
                for (int j = i + 1; j < balls.Count; j++)
                {
                    IBall b1 = balls[i];
                    IBall b2 = balls[j];

                    double dx = b2.position.X - b1.position.X;
                    double dy = b2.position.Y - b1.position.Y;
                    double distance = Math.Sqrt(dx * dx + dy * dy);
                    double minDistance = b1.radius + b2.radius;

                    if (distance <= minDistance)
                    {
                        Vector2D relVel = new Vector2D(b2.velocity.X - b1.velocity.X, b2.velocity.Y - b1.velocity.Y);
                        double dotProduct = dx * relVel.X + dy * relVel.Y;

                        if (dotProduct < 0)
                        {
                            ResolveCollision(b1, b2);
                        }
                    }
                }
            }
        }

        private void HandleWallCollisions()
        {
            foreach (var ball in balls)
            {
                lock (ball.lockObject)
                {
                    double newX = ball.position.X;
                    double newY = ball.position.Y;
                    double vx = ball.velocity.X;
                    double vy = ball.velocity.Y;

                    bool changed = false;

                    if (newX - ball.radius <= 0)
                    {
                        vx = Math.Abs(vx);
                        newX = ball.radius;
                        changed = true;
                    }
                    else if (newX + ball.radius >= board.width)
                    {
                        vx = -Math.Abs(vx);
                        newX = board.width - ball.radius;
                        changed = true;
                    }

                    if (newY - ball.radius <= 0)
                    {
                        vy = Math.Abs(vy);
                        newY = ball.radius;
                        changed = true;
                    }
                    else if (newY + ball.radius >= board.height)
                    {
                        vy = -Math.Abs(vy);
                        newY = board.height - ball.radius;
                        changed = true;
                    }

                    if (changed)
                    {
                        ball.SetVelocity(new Vector2D(vx, vy));
                        ball.SetPosition(new Vector2D(newX, newY));

                    }
                }
            }
        }

        private void ResolveCollision(IBall b1, IBall b2)
        {

            double m1 = b1.weight;
            double m2 = b2.weight;

            Vector2D v1 = b1.velocity;
            Vector2D v2 = b2.velocity;
            Vector2D p1 = b1.position;
            Vector2D p2 = b2.position;

            double dx = p1.X - p2.X;
            double dy = p1.Y - p2.Y;
            double distSq = dx * dx + dy * dy;

            if (distSq == 0) return;

            double dot = (v1.X - v2.X) * dx + (v1.Y - v2.Y) * dy;
            double scalar = (2 * m2 / (m1 + m2)) * (dot / distSq);

            b1.SetVelocity(new Vector2D(v1.X - scalar * dx, v1.Y - scalar * dy));

            dot = (v2.X - v1.X) * (-dx) + (v2.Y - v1.Y) * (-dy);
            scalar = (2 * m1 / (m1 + m2)) * (dot / distSq);

            b2.SetVelocity(new Vector2D(v2.X - scalar * (-dx), v2.Y - scalar * (-dy)));
        }
    }
}