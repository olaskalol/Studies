using System;
using System.Threading;

namespace Data
{
    public interface IBall 
    {
        Vector2D position { get; }
        void SetPosition(Vector2D newPosition);
        Vector2D velocity { get; }
        void SetVelocity(Vector2D newVelocity);

        double radius { get; }
        double weight { get; }

        object lockObject { get; }

        event EventHandler BallChanged;
        void StartMoving(CancellationToken token);
    }
}
