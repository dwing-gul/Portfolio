using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyChaseTrigger : MonoBehaviour
{
    [SerializeField]
    private Enemy Owner;

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Player") || collision.gameObject.CompareTag("Shield"))
        {
            Owner.SetTarget(collision.transform);
        } 
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if(collision.gameObject.CompareTag("Player") || collision.gameObject.CompareTag("Shield"))
        {
            Owner.RemoveTarget();
        }
    }
}
