using UnityEngine;
using UnityEngine.InputSystem; // This line is required for the new system

public class BowScript : MonoBehaviour
{
    void Update()
    {
        // 1. Get the mouse position using the NEW system
        Vector2 mouseScreenPosition = Mouse.current.position.ReadValue();

        // 2. Convert to world position
        Vector3 mouseWorldPosition = Camera.main.ScreenToWorldPoint(new Vector3(mouseScreenPosition.x, mouseScreenPosition.y, 10f));

        // 3. Calculate the direction from the bow to the mouse
        Vector2 direction = new Vector2(
            mouseWorldPosition.x - transform.position.x,
            mouseWorldPosition.y - transform.position.y
        );

        // 4. Set the rotation (Z-axis for 2D)
        float angle = Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg;
        transform.rotation = Quaternion.Euler(0, 0, angle);
    }
}