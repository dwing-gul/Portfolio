using System.Collections;
using System.Collections.Generic;
using System.Numerics;
using UnityEngine;

public class PhysicsObject : MonoBehaviour
{
    public float minGroundNormalY = .65f;
    public float gravityModifier = 1f;

    protected UnityEngine.Vector2 targetVelocity;
    protected bool grounded;
    protected UnityEngine.Vector2 groundNormal;
    protected Rigidbody2D rigidbody2D;
    protected UnityEngine.Vector2 velocity;
    protected ContactFilter2D contactFilter;
    protected RaycastHit2D[] hitBuffer = new RaycastHit2D[16];
    protected List<RaycastHit2D> hitBufferList = new List<RaycastHit2D>(16);

    protected const float minMoveDistance = 0.001f;
    protected const float shellRadius = 0.001f;

    void OnEnable()
    {
        rigidbody2D = GetComponent<Rigidbody2D>();
    }

    // Start is called before the first frame update
    void Start()
    {
        contactFilter.useTriggers = false;
        contactFilter.SetLayerMask(Physics2D.GetLayerCollisionMask(gameObject.layer));
        contactFilter.useLayerMask = true;
    }

    // Update is called once per frame
    void Update()
    {
        targetVelocity = UnityEngine.Vector2.zero;
        ComputeVelocity();
    }
    protected virtual void ComputeVelocity()
    {

    }

    private void FixedUpdate()
    {
        velocity += gravityModifier * Physics2D.gravity * Time.deltaTime;
        grounded = false;
        UnityEngine.Vector2 deltaPosition = velocity * Time.deltaTime;
        UnityEngine.Vector2 move = UnityEngine.Vector2.up * deltaPosition.y;
        Movement(move, true);
    }

    void Movement(UnityEngine.Vector2 move, bool yMovement)
    {
        float distance = move.magnitude;

        if(distance > minMoveDistance)
        {
            int count = rigidbody2D.Cast(move, contactFilter, hitBuffer, distance + shellRadius);
            hitBufferList.Clear();

            for (int i = 0; i < count; i++)
            {
                hitBufferList.Add(hitBuffer[i]);
            }

            for(int i = 0; i < hitBufferList.Count; i++)
            {
                UnityEngine.Vector2 currentNormal = hitBufferList[i].normal;
                if(currentNormal.y > minGroundNormalY)
                {
                    grounded = true;

                    if(yMovement)
                    {
                        groundNormal = currentNormal;
                        currentNormal.x = 0;
                    }
                }

                float projection = UnityEngine.Vector2.Dot(velocity, currentNormal);
                if (projection < 0)
                {
                    velocity = velocity - projection * currentNormal;
                }

                float modifiedDistance = hitBufferList[i].distance - shellRadius;
                distance = modifiedDistance < distance ? modifiedDistance : distance;
            }
        }
        rigidbody2D.position = rigidbody2D.position + move;
    }
}
