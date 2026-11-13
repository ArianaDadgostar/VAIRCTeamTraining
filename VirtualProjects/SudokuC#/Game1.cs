using System.Security.Cryptography.X509Certificates;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace SudokuC_;

public class Game1 : Game
{
    /*

        = new int[] {3, 0, 0},
                                new int[] {0, 1, 0},
                                new int[] {1, 0, 2};
    */
    private GraphicsDeviceManager _graphics;
    private SpriteBatch spriteBatch;

    private Texture2D line;

    public Game1()
    {
        _graphics = new GraphicsDeviceManager(this);
        Content.RootDirectory = "Content";
        IsMouseVisible = true;
    }

    protected override void Initialize()
    {
        // TODO: Add your initialization logic here

        line = new Texture2D(GraphicsDevice, 1, 1);
        line.SetData(new[] { Color.White });

        base.Initialize();
    }

    protected override void LoadContent()
    {
        spriteBatch = new SpriteBatch(GraphicsDevice);

        // TODO: use this.Content to load your game content here
    }

    protected override void Update(GameTime gameTime)
    {
        if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
            Exit();

        // TODO: Add your update logic here

        base.Update(gameTime);
    }

    private void DrawBoard()
    {
        int width = base.Window.ClientBounds.Width;
        int height = base.Window.ClientBounds.Height;

        spriteBatch.Draw(line, new Rectangle(width / 3, 0, 5, height), Color.BlueViolet);
        spriteBatch.Draw(line, new Rectangle(width * 2 / 3, 0, 5, height), Color.BlueViolet);

        spriteBatch.Draw(line, new Rectangle(0, height / 3, width, 5), Color.BlueViolet);
        spriteBatch.Draw(line, new Rectangle(0, height * 2 / 3, width, 5), Color.BlueViolet);
    }

    protected override void Draw(GameTime gameTime)
    {
        spriteBatch.Begin();
        GraphicsDevice.Clear(Color.FloralWhite);

        DrawBoard();

        // TODO: Add your drawing code here

        base.Draw(gameTime);
        spriteBatch.End();
    }
}
