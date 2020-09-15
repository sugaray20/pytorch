#pragma once

#include <torch/csrc/autograd/variable.h>

#include <ATen/ATen.h>

namespace torch {
namespace autograd {

/// Computes the sum of gradients of given tensors with respect to graph leaves.
///
/// The graph is differentiated using the chain rule. If any of ``tensors``
/// are non-scalar (i.e. their data has more than one element) and require gradient,
/// then the Jacobian-vector product would be computed, in this case the function
/// additionally requires specifying `grad_tensors`. It should be a sequence of
/// matching length, that contains the "vector" in the Jacobian-vector product,
/// usually the gradient of the differentiated function w.r.t. corresponding tensors
/// (`torch::Tensor()` is an acceptable value for all tensors that don't need
/// gradient tensors).
///
/// This function accumulates gradients in the leaves - you might need to zero them
/// before calling it.
///
/// \param tensors Tensors of which the derivative will be computed.
/// \param grad_tensors The "vector" in the Jacobian-vector product, usually gradients
///     w.r.t. each element of corresponding tensors. `torch::Tensor()` values can be
///     specified for scalar Tensors or ones that don't require grad. If a `torch::Tensor()` value
///     would be acceptable for all grad_tensors, then this argument is optional.
/// \param retain_graph If `false`, the graph used to compute the grad will be freed.
///     Note that in nearly all cases setting this option to `true` is not needed
///     and often can be worked around in a much more efficient way. Defaults to the
///     value of `create_graph`.
/// \param create_graph If `true`, graph of the derivative will be constructed, allowing
///     to compute higher order derivative products. Defaults to `false`.
TORCH_API void backward(
    const variable_list& tensors,
    const variable_list& grad_tensors = {},
    c10::optional<bool> retain_graph = c10::nullopt,
    bool create_graph = false);

/// Computes and returns the sum of gradients of outputs with respect to the inputs.
///
/// ``grad_outputs`` should be a sequence of length matching ``output``
/// containing the "vector" in Jacobian-vector product, usually the pre-computed
/// gradients w.r.t. each of the outputs. If an output doesn't require_grad,
/// then the gradient can be ``torch::Tensor()``).
///
/// \param outputs outputs of the differentiated function.
/// \param inputs Inputs w.r.t. which the gradient will be
///     returned (and not accumulated into ``at::Tensor::grad``).
/// \param grad_outputs The "vector" in the Jacobian-vector product.
///     Usually gradients w.r.t. each output. `torch::Tensor()` values can be specified for scalar
///     Tensors or ones that don't require grad. If a `torch::Tensor()` value would be acceptable
///     for all grad_tensors, then this argument is optional. Default: `{}`.
/// \param retain_graph If ``false``, the graph used to compute the grad
///     will be freed. Note that in nearly all cases setting this option to ``true``
///     is not needed and often can be worked around in a much more efficient
///     way. Defaults to the value of ``create_graph``.
/// \param create_graph If ``true``, graph of the derivative will
///     be constructed, allowing to compute higher order derivative products.
///     Default: ``false``.
/// \param allow_unused If ``false``, specifying inputs that were not
///     used when computing outputs (and therefore their grad is always zero)
///     is an error. Defaults to ``false``.
TORCH_API variable_list grad(
    const variable_list& outputs,
    const variable_list& inputs,
    const variable_list& grad_outputs = {},
    c10::optional<bool> retain_graph = c10::nullopt,
    bool create_graph = false,
    bool allow_unused = false);

namespace forward_ad {

TORCH_API uint64_t enter_dual_level();
TORCH_API void exit_dual_level(int64_t level);

TORCH_API at::Tensor make_dual(const at::Tensor& primal, at::Tensor tangent, int64_t level=0);
TORCH_API std::pair<at::Tensor, at::Tensor> unpack_dual(at::Tensor tensor, int64_t level=0);

} // namespace forward_ad
} // namespace autograd
} // namespace torch
