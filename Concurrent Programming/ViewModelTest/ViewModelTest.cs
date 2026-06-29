using Microsoft.VisualStudio.TestTools.UnitTesting;
using ViewModel;
using Model;
using Logic;
using Data;
using System;
using System.Collections.Generic;
using System.Threading;

namespace ViewModelTest
{
    internal class FakeBall : IBall
    {
        public Vector2D position { get; private set; } = new Vector2D(0, 0);
        public Vector2D velocity { get; private set; } = new Vector2D(0, 0);
        public double radius { get; } = 15;
        public double weight { get; } = 10;
        public object lockObject { get; } = new object();

        public event EventHandler BallChanged;
        public void SetVelocity(Vector2D newVelocity) => velocity = newVelocity;
        public void SetPosition(Vector2D newPosition) => position = newPosition;
        public void StartMoving(CancellationToken token) { }
        public void Move(double newX, double newY)
        {
            SetPosition(new Vector2D(newX, newY));
            BallChanged?.Invoke(this, EventArgs.Empty);
        }
    }

    internal class FakeLogicForViewModel : LogicAbstractApi
    {
        public override event EventHandler PositionChanged;
        public override void StartSimulation(int numberOfBalls) { }
        public override void StopSimulation() { }

        public override IEnumerable<IBall> GetBalls()
        {
            return new List<IBall> { new FakeBall(), new FakeBall(), new FakeBall() };
        }
    }

    [TestClass]
    public class MainViewModelTest
    {
        [TestMethod]
        public void StartCommand_PopulatesBallsCollection()
        {
            FakeLogicForViewModel fakeLogic = new FakeLogicForViewModel();
            MainModel model = new MainModel(fakeLogic);
            MainViewModel viewModel = new MainViewModel(model);
            viewModel.NumberOfBalls = 3;

            viewModel.StartCommand.Execute(null);

            Assert.AreEqual(3, viewModel.Balls.Count, "Kolekcja Balls powinna zawierać 3 elementy pochodzące z Logiki.");
        }

        [TestMethod]
        public void NumberOfBalls_PropertyChange_RaisesEvent()
        {
            FakeLogicForViewModel fakeLogic = new FakeLogicForViewModel();
            MainModel model = new MainModel(fakeLogic);
            MainViewModel viewModel = new MainViewModel(model);
            bool propertyChangedRaised = false;

            viewModel.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == nameof(viewModel.NumberOfBalls))
                    propertyChangedRaised = true;
            };
            viewModel.NumberOfBalls = 10;

            Assert.IsTrue(propertyChangedRaised, "Zmiana NumberOfBalls powinna wywołać zdarzenie PropertyChanged.");
        }

        [TestMethod]
        public void BallModel_UpdatesProperties_When_LogicBall_Moves()
        {
            FakeBall fakeBall = new FakeBall();
            fakeBall.SetPosition(new Vector2D(10, 10));

            BallModel ballModel = new BallModel(fakeBall);
            bool isNotified = false;
            ballModel.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == "X" || e.PropertyName == "Y")
                    isNotified = true;
            };
            fakeBall.Move(50, 50);

            Assert.IsTrue(isNotified, "BallModel powinien zgłosić do XAML, że zmienił się X lub Y.");
            Assert.AreEqual(50, ballModel.X, "BallModel powinien zaktualizować swoją wartość X.");
            Assert.AreEqual(50, ballModel.Y, "BallModel powinien zaktualizować swoją wartość Y.");
        }
    }
}