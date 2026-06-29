using System;
using System.Diagnostics;
using System.Numerics;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
namespace Data
{
    internal class Ball : IBall
    {
        public Vector2D position { get; private set; }
        public Vector2D velocity { get; private set; }
        public double radius { get; }
        public double weight { get; }
        public object lockObject { get; } = new object();

        public event EventHandler BallChanged;

        private System.Timers.Timer movementTimer;
        private Stopwatch stopwatch;

        public Ball(double Px, double Py, double Vx, double Vy, double inRadius , double inWeight)
        {
            position = new Vector2D(Px, Py);
            velocity = new Vector2D(Vx, Vy);
            radius = inRadius;
            weight = inWeight;
        }

        public void SetPosition(Vector2D newPosition)
        {
            lock (lockObject)
            {
                position = newPosition;
            }
        }
        public void SetVelocity(Vector2D newVelocity)
        {
            lock (lockObject)
            {
                velocity = newVelocity;
            }
        }

        public void StartMoving(CancellationToken token)
        {
            stopwatch = new Stopwatch();
            stopwatch.Start();

            movementTimer = new System.Timers.Timer(10);
            

            movementTimer.Elapsed += async (sender, e) =>
            {
                if (token.IsCancellationRequested)
                {
                    stopwatch.Stop();
                    return;
                }

                await Task.Run(() => UpdateBallState(), token);

                if (!token.IsCancellationRequested)
                {
                    movementTimer.Start();
                }
            };

            token.Register(() =>
            {
                movementTimer?.Stop();
                movementTimer?.Dispose();
            });

            movementTimer.Start();
        }

        private void UpdateBallState()
        {
            double timeElapsed = stopwatch.Elapsed.TotalSeconds;
            stopwatch.Restart();

            lock (lockObject)
            {
                double newX = position.X + (velocity.X * timeElapsed * 50);
                double newY = position.Y + (velocity.Y * timeElapsed * 50);
                position = new Vector2D(newX, newY);
            }

            BallChanged?.Invoke(this, EventArgs.Empty);
            DataAbstractApi.logger.LogBallState(this);
        }
    }
}
